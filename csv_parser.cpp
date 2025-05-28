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

extern "C" {
    void processCSVBuildings(const char* csvData) {
        string data(csvData);
        stringstream ss(data);
        string line;

        int HullCounter = -1;

        while (getline(ss, line)) {
            stringstream lineStream(line);
            string * row = new string[ROW_LENGTH];

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

val getNodeCoordinates(int camX, int camY, double zoom, int canvasWidth, int canvasHeight) {
    val arr = val::array();

    for (size_t i = 0; i < objectKingdom.nodeVector.size(); ++i) {
        auto node = objectKingdom.nodeVector[i];

        double worldX = node->getX();
        double worldY = node->getY();

        // Translacja świata do ekranu
        double screenX = (worldX - static_cast<double>(camX)) * zoom + static_cast<double>(canvasWidth) * 0.5;
        double screenY = (worldY - static_cast<double>(camY)) * zoom + static_cast<double>(canvasHeight) * 0.5;

        // Pomijanie punktów niewidocznych
        if (screenX < 0 || screenX > canvasWidth || screenY < 0 || screenY > canvasHeight)
            continue;

        val obj = val::object();
        obj.set("x", screenX);
        obj.set("y", screenY);
        obj.set("radius", CONST_RADIUS * zoom); // Skalowanie promienia
        obj.set("name", node->getName());

        arr.call<void>("push", obj);
    }
    return arr;
}

val getRelations(int camX, int camY, double zoom, int canvasWidth, int canvasHeight) {
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

            arr.call<void>("push", obj);
        }
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



EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("getNodeCoordinates", &getNodeCoordinates);
    emscripten::function("getRelations", &getRelations);
    emscripten::function("getHulls", &getHulls);
    emscripten::function("calculateFlow", &calculateFlow);
}
