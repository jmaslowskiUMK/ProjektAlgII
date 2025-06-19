#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <emscripten/bind.h>
#include "./sourceFiles/Country.h"
#include "./sourceFiles/Brewery.h"
#include "./sourceFiles/Pub.h"
#include "./sourceFiles/Field.h"
#include "./sourceFiles/Brewery.h"
#include "./sourceFiles/Hull.h"

#define ROW_LENGTH 12
#define CONST_RADIUS 10

using namespace emscripten;
using namespace std;

Country objectKingdom;


// counters for manual creation
int fieldsCounter = 0;
int breweriesCounter = 0;
int pubsCounter = 0;


extern "C" {
    void processCSVBuildings(const char* csvData) {

        // destruction of manually created kingdom
        //objectKingdom.~Country(); // destruct
        //new(&objectKingdom) Country(); // reconstruct

        objectKingdom.reset();
        fieldsCounter = 0;
        pubsCounter = 0;
        breweriesCounter = 0;


        string data(csvData);
        stringstream ss(data);
        string line;

        int HullCounter = -1;

        while (getline(ss, line)) {
            stringstream lineStream(line);
            //string * row = new string[ROW_LENGTH];
            vector <string> row(ROW_LENGTH);

            for (int i = 0; i < ROW_LENGTH; i++) {
                getline(lineStream, row[i], ',');
            }

            if (row[0] == "Hull_" + to_string(HullCounter + 1)) {
                // Function to create Hull object and add to vector
                objectKingdom.addHull(stoi(row[11]));
                HullCounter += 1;
            }   else if (row[0].find("HullPoint") != std::string::npos) {
                // Function to add to Hull
                objectKingdom.hulls[HullCounter]->addPoint(std::pair<int, int> (stoi(row[5]), stoi(row[6])));
            }   else if (row[0] == "Field") {
                // adding Field
                objectKingdom.createField(stoi(row[1]), stoi(row[2]), stoi(row[5]), stoi(row[6]), CONST_RADIUS);
            }   else if (row[0] == "Brewery") {
                // adding Brewery
                objectKingdom.createBrewery(stoi(row[1]), stoi(row[5]), stoi(row[6]), CONST_RADIUS, stoi(row[3]));
            }   else if (row[0] == "Pub") {
                // adding Pub
                objectKingdom.createPub(stoi(row[1]), stoi(row[5]), stoi(row[6]), CONST_RADIUS);
            }   else if (row[0] == "Lane") {
                auto from = objectKingdom.find(stoi(row[7]));
                auto to = objectKingdom.find(stoi(row[8]));

                if (from && to) {
                    Lane lane(from, to, stod(row[9]), stoi(row[10]));
                    objectKingdom.addRelationship(lane);
                }
            }
        }
    }
}

val getRelationsAndCoordinates(int camX, int camY, double zoom, int canvasWidth, int canvasHeight) {
    val arr = val::array();

    for (auto& pair : objectKingdom.adjList) {
        auto fromNode = pair.first;
        auto& lanes = pair.second;

        double fromWorldX = fromNode->getX();
        double fromWorldY = fromNode->getY();
        double fromScreenX = (fromWorldX - static_cast<double>(camX)) * zoom + static_cast<double>(canvasWidth) * 0.5;
        double fromScreenY = (fromWorldY - static_cast<double>(camY)) * zoom + static_cast<double>(canvasHeight) * 0.5;

        for (Lane& lane : lanes) {
            auto toNode = lane.getToPtr();

            double toWorldX = toNode->getX();
            double toWorldY = toNode->getY();
            double toScreenX = (toWorldX - static_cast<double>(camX)) * zoom + static_cast<double>(canvasWidth) * 0.5;
            double toScreenY = (toWorldY - static_cast<double>(camY)) * zoom + static_cast<double>(canvasHeight) * 0.5;

            if ((fromScreenX < 0 && toScreenX < 0) || (fromScreenX > canvasWidth && toScreenX > canvasWidth) ||
                (fromScreenY < 0 && toScreenY < 0) || (fromScreenY > canvasHeight && toScreenY > canvasHeight))
                continue;

            val obj = val::object();
            obj.set("startX", fromScreenX);
            obj.set("startY", fromScreenY);
            obj.set("endX", toScreenX);
            obj.set("endY", toScreenY);
            obj.set("capacity", to_string(lane.getCapacity()));
            obj.set("startName", lane.getFromPtr()->getName());
            obj.set("endName", lane.getToPtr()->getName());
            obj.set("startID", lane.getFromPtr()->getID());
            obj.set("endID", lane.getToPtr()->getID());
            obj.set("radius", CONST_RADIUS * zoom);



            arr.call<void>("push", obj);
        }
    }
    return arr;
}

val getNoRelationsCoordinates(int camX, int camY, double zoom, int canvasWidth, int canvasHeight) {
    val arr = val::array();

    for (const auto& node : objectKingdom.nodeVector) {
        double worldX = node->getX();
        double worldY = node->getY();

        double screenX = (worldX - static_cast<double>(camX)) * zoom + static_cast<double>(canvasWidth) * 0.5;
        double screenY = (worldY - static_cast<double>(camY)) * zoom + static_cast<double>(canvasHeight) * 0.5;

        // opcjonalne: pomiń niewidoczne elementy
        if (screenX < 0 || screenX > canvasWidth || screenY < 0 || screenY > canvasHeight)
            continue;

        val obj = val::object();
        obj.set("x", screenX);
        obj.set("y", screenY);
        obj.set("name", node->getName());
        obj.set("ID", node->getID());
        obj.set("radius", CONST_RADIUS * zoom);

        arr.call<void>("push", obj);
    }

    return arr;
}



val getHulls(int camX, int camY, double zoom, int canvasWidth, int canvasHeight) {
    val hullArray = val::array();

    for (size_t i = 0; i < objectKingdom.hulls.size(); ++i) {
        auto hull = objectKingdom.hulls[i];
        val pointArray = val::array();
        bool hasVisiblePoint = false;

        int index = 0;
        for (const auto& point : hull->points) {
            double worldX = static_cast<double>(point.first);
            double worldY = static_cast<double>(point.second);

            double screenX = (worldX - static_cast<double>(camX)) * zoom + static_cast<double>(canvasWidth) * 0.5;
            double screenY = (worldY - static_cast<double>(camY)) * zoom + static_cast<double>(canvasHeight) * 0.5;

            // Tworzymy punkt zawsze
            val pointObj = val::object();
            pointObj.set("x", screenX);
            pointObj.set("y", screenY);
            pointArray.set(index++, pointObj);

            // Sprawdzamy widoczność
            if (screenX >= 0 && screenX <= canvasWidth && screenY >= 0 && screenY <= canvasHeight) {
                hasVisiblePoint = true;
            }
        }

        // Dodajemy tylko jeśli przynajmniej jeden punkt był widoczny
        if (hasVisiblePoint) {
            val hullObj = val::object();
            hullObj.set("points", pointArray);
            hullObj.set("groundClass", hull->groundClass);
            hullArray.call<void>("push", hullObj);
        }
    }

    return hullArray;
}



val calculateFlow() {
	val resultsArr = val::array();

    std::vector<std::shared_ptr<Node>> sinks;
	std::vector<std::shared_ptr<Node>> sources;
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Field>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Pub>(el)) {
			sinks.push_back(el);
		}
	}

    resultsArr.call<void>("push", objectKingdom.edmondsKarpManyToMany(sources,sinks));
    resultsArr.call<void>("push", objectKingdom.dinic(sources,sinks));

    return resultsArr;
}


void createField(int xMiddle, int yMiddle) {
    // production, for now - default
    int production = 2137;

    objectKingdom.createField(fieldsCounter * 3 + 0, production, xMiddle, yMiddle, CONST_RADIUS);
    fieldsCounter += 1;

    cout << "field creation succesfull: " << objectKingdom.nodeVector[0]->getName() << endl;
}

void createBrewery(int xMiddle, int yMiddle) {
    // barley, for now - default
    int barleyAmount = 2137;

    objectKingdom.createBrewery(breweriesCounter * 3 + 1, xMiddle, yMiddle, CONST_RADIUS, barleyAmount);
    breweriesCounter += 1;

    cout << "brewery creation succesfull: " << objectKingdom.nodeVector[0]->getName() << endl;
}

void createPub(int xMiddle, int yMiddle) {
    objectKingdom.createPub(pubsCounter * 3 + 2, xMiddle, yMiddle, CONST_RADIUS);
    pubsCounter += 1;

    cout << "pub creation succesfull: " << objectKingdom.nodeVector[0]->getName() << endl;
}

void createRelation(int firstID, int secoundID, int capacity, int repair_cost) {
    // for now capacity and repair cost are default

    auto from = objectKingdom.find(firstID);
    auto to = objectKingdom.find(secoundID);

    if (from && to) {
        Lane lane(from, to, capacity, repair_cost);
        objectKingdom.addRelationship(lane);
    }
}

bool isInAnyHull(int x, int y) {
    for (int i = 0; i < objectKingdom.hulls.size(); ++i) {
        if (objectKingdom.rayCasting(objectKingdom.hulls[i]->points, std::make_pair(x, y))) {
            return true;
        }
    }
    return false;
}


EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("getRelationsAndCoordinates", &getRelationsAndCoordinates);
    emscripten::function("getHulls", &getHulls);
    emscripten::function("calculateFlow", &calculateFlow);

    // create object functions
    emscripten::function("createField", &createField);
    emscripten::function("createBrewery", &createBrewery);
    emscripten::function("createPub", &createPub);
    emscripten::function("createRelation", &createRelation);

    // helper functions
    emscripten::function("getNoRelationsCoordinates", &getNoRelationsCoordinates);
    emscripten::function("isInAnyHull", &isInAnyHull);
}