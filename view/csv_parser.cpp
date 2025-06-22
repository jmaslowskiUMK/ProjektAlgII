#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <emscripten/bind.h>
#include "../sourceFiles/Country.h"
#include "../sourceFiles/Brewery.h"
#include "../sourceFiles/Pub.h"
#include "../sourceFiles/Field.h"
#include "../sourceFiles/Brewery.h"
#include "../sourceFiles/Hull.h"

#define ROW_LENGTH 14
#define CONST_RADIUS 10

using namespace emscripten;
using namespace std;

Country objectKingdom;

// counters for manual creation
int fieldsCounter = 0;
int breweriesCounter = 0;
int pubsCounter = 0;
int HullCounter = -1;


extern "C" {
    void processCSVBuildings(const char* csvData) {

        objectKingdom.reset();
        fieldsCounter = 0;
        pubsCounter = 0;
        breweriesCounter = 0;


        string data(csvData);
        stringstream ss(data);
        string line;

        HullCounter = -1;

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
                fieldsCounter += 1;
            }   else if (row[0] == "Brewery") {
                // adding Brewery
                objectKingdom.createBrewery(stoi(row[1]), stoi(row[5]), stoi(row[6]), CONST_RADIUS, stoi(row[3]));
                breweriesCounter += 1;
            }   else if (row[0] == "Pub") {
                // adding Pub
                objectKingdom.createPub(stoi(row[1]), stoi(row[5]), stoi(row[6]), CONST_RADIUS, stoi(row[4]));
                pubsCounter += 1;
            }   else if (row[0] == "Lane") {
                auto from = objectKingdom.find(stoi(row[7]));
                auto to = objectKingdom.find(stoi(row[8]));

                if (from && to) {
                    Lane lane(from, to, stod(row[9]), stoi(row[10]));
                    objectKingdom.addRelationship(lane);
                }
            }   else if (row[0] == "Seed") {
                objectKingdom.setSeed(row[13]);
            }   else if (row[0] == "convRate") {
                objectKingdom.setConvRate(stoi(row[12]));
            }
        }

        if (objectKingdom.getConvRate() == 0) {
            objectKingdom.setConvRate(1);
        }
        if (objectKingdom.getSeed() == "") {
            objectKingdom.setSeed("DEFAULT");
        }
        /*
        std::sort(objectKingdom.nodeVector.begin(), objectKingdom.nodeVector.end(),
        [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {

            return a->getID() < b->getID();

        });
        */


        // process to determine field production
        for (int i = 0; i < objectKingdom.nodeVector.size(); i += 3) {
            for (int j = 0; j < objectKingdom.hulls.size(); j++) {
                if (objectKingdom.rayCasting(objectKingdom.hulls[j]->points, std::make_pair(objectKingdom.nodeVector[i]->getX(), objectKingdom.nodeVector[i]->getY()))) {
                    shared_ptr<Field> derived = std::dynamic_pointer_cast<Field>(objectKingdom.nodeVector[i]);
                    if (derived != nullptr) {
                        // scale of grounds here
                        derived->setProduction(
                            static_cast<int>(static_cast<double>(derived->getProduction() * (1 + static_cast<double>(objectKingdom.hulls[j]->groundClass - 3) / 10.0)))
                        );
                    }
                }
            }
        }

    }
}


string saveToCSV() {
    std::stringstream ss;
    ss << "Category,ID,Yield (kg),Processed (kg),Beer (liters),X Coordinate,Y Coordinate,Lane From,Lane To,Capacity (kg/liters),Repair Cost,Ground Class\n";

    // Nodes: Field, Brewery, Pub
    for (const auto& node : objectKingdom.nodeVector) {
        int id = node->getID();
        int x = node->getX();
        int y = node->getY();

        if (auto field = std::dynamic_pointer_cast<Field>(node)) {
            ss << "Field," << id << "," << field->getBasicProduction()
               << ",,,"
               << x << "," << y << ",,,,,\n";

        } else if (auto brewery = std::dynamic_pointer_cast<Brewery>(node)) {
            ss << "Brewery," << id << ",," << brewery->getBarleyCap()
               << ",," << x << "," << y << ",,,,,\n";

        } else if (auto pub = std::dynamic_pointer_cast<Pub>(node)) {
            ss << "Pub," << id << ",,," << pub->getCapacity()
               << "," << x << "," << y << ",,,,,\n";
        }
    }

    // Lanes
    for (auto& [fromNode, lanes] : objectKingdom.adjList) {
        for (auto& lane : lanes) {
            auto from = lane.getFromPtr();
            auto to = lane.getToPtr();
            if (!from || !to) continue;

            ss << "Lane,,,,,,,"
               << from->getID() << "," << to->getID()
               << "," << lane.getCapacity()
               << "," << lane.getRepairCost()
               << ",\n";
        }
    }


    // Hulls and HullPoints
    for (size_t i = 0; i < objectKingdom.hulls.size(); ++i) {
        const auto& hull = objectKingdom.hulls[i];
        ss << "Hull_" << i << ",Array of Points,,,,,,,,,," << hull->groundClass << "\n";

        for (size_t j = 0; j < hull->points.size(); ++j) {
            auto& p = hull->points[j];
            ss << "HullPoint_" << i << "," << j << ",,,," << p.first << "," << p.second << ",,,,,"
               << hull->groundClass << "\n";
        }
    }

    ss << "ConvRate,,,,,,,,,,,," << objectKingdom.getConvRate() << ",\n";
    ss << "Seed,,,,,,,,,,,,," << objectKingdom.getSeed() << "\n";

    return ss.str();
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
        auto it = objectKingdom.adjList.find(node);
        if (it != objectKingdom.adjList.end() && !it->second.empty()) {
            continue;
        }

        double worldX = node->getX();
        double worldY = node->getY();

        double screenX = (worldX - static_cast<double>(camX)) * zoom + static_cast<double>(canvasWidth) * 0.5;
        double screenY = (worldY - static_cast<double>(camY)) * zoom + static_cast<double>(canvasHeight) * 0.5;

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

val getNodesCoordinates(int camX, int camY, double zoom, int canvasWidth, int canvasHeight) {
    val arr = val::array();

    for (const auto& node : objectKingdom.nodeVector) {
        double worldX = node->getX();
        double worldY = node->getY();

        double screenX = (worldX - static_cast<double>(camX)) * zoom + static_cast<double>(canvasWidth) * 0.5;
        double screenY = (worldY - static_cast<double>(camY)) * zoom + static_cast<double>(canvasHeight) * 0.5;

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
    /*
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
    */
    val resultsArr = val::array();
    std::vector<std::shared_ptr<Node>> sinks;
	std::vector<std::shared_ptr<Node>> sources;

	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Field>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sinks.push_back(el);
		}

	}
        int time =0 ;
        std::pair<int,int> para =  objectKingdom.fordFulkerson(sources,sinks, 1);
	std::cout << "1: " <<para.first<<std::endl;
        time+=para.second;
	sources.clear();
	sinks.clear();

	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Pub>(el)) {
			sinks.push_back(el);
		}

	}

        para =  objectKingdom.fordFulkerson(sources,sinks, 1);
        resultsArr.call<void>("push", para.first);
        time+=para.second; 
        resultsArr.call<void>("push", time);


        sources.clear();
        sinks.clear();
        time=0;


	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Field>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sinks.push_back(el);
		}

	}
        para =  objectKingdom.edmondsKarpManyToMany(sources,sinks, 1);
	std::cout << "1: " <<para.first<<std::endl;
        time+=para.second;
	sources.clear();
	sinks.clear();

	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Pub>(el)) {
			sinks.push_back(el);
		}

	}

        para = objectKingdom.edmondsKarpManyToMany(sources,sinks, 1);
        time+=para.second;
        resultsArr.call<void>("push", para.first);
        resultsArr.call<void>("push", time);
        sources.clear();
        sinks.clear();
    
        time=0;

	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Field>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sinks.push_back(el);
		}

	}
        
        para=objectKingdom.dinic(sources,sinks, 1);
	std::cout << "1: " <<para.first<<std::endl;
        time+=para.second;
	sources.clear();
	sinks.clear();

	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Pub>(el)) {
			sinks.push_back(el);
		}

	}

        para=objectKingdom.dinic(sources,sinks, 1);
        time+=para.second;
        resultsArr.call<void>("push", para.first);
        resultsArr.call<void>("push", time);
        sources.clear();
        sinks.clear();   



         for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Field>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sinks.push_back(el);
		}

	}

    std::cout << "2" << endl;
    /*
	int x=objectKingdom.mcmf(sources,sinks, 1).first;

	sources.clear();
	sinks.clear();

        for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Brewery>(el)) {
			sources.push_back(el);
		}
	}
	for(auto el:objectKingdom.nodeVector){
		if (auto derived = std::dynamic_pointer_cast<Pub>(el)) {
			sinks.push_back(el);
		}

	}

        x+=objectKingdom.mcmf(sources,sinks, 1).first;
        resultsArr.call<void>("push",x);
        sources.clear();
        sinks.clear();
*/


    return resultsArr;
}


// ============================================================================
// ===                            Manual Creation                           ===
// ============================================================================

void createField(int xMiddle, int yMiddle, int production) {
    // add reycasting
    shared_ptr<Field> field = objectKingdom.createField(fieldsCounter * 3 + 0, production, xMiddle, yMiddle, CONST_RADIUS);
    fieldsCounter += 1;

    for (int j = 0; j < objectKingdom.hulls.size(); j++) {
        if (objectKingdom.rayCasting(objectKingdom.hulls[j]->points, std::make_pair(xMiddle, yMiddle))) {
            // scale of grounds here
            field->setProduction(
                static_cast<int>(static_cast<double>(field->getProduction() * (1 + static_cast<double>(objectKingdom.hulls[j]->groundClass - 3) / 10.0)))
            );
            break;
        }
    }

    cout << "field creation succesfull: " << endl;
}

void createBrewery(int xMiddle, int yMiddle, int barleyCap) {

    objectKingdom.createBrewery(breweriesCounter * 3 + 1, xMiddle, yMiddle, CONST_RADIUS, barleyCap);
    breweriesCounter += 1;

    cout << "brewery creation succesfull: " << endl;
}

void createPub(int xMiddle, int yMiddle, int capacity) {
    objectKingdom.createPub(pubsCounter * 3 + 2, xMiddle, yMiddle, CONST_RADIUS, capacity);
    pubsCounter += 1;

    cout << "pub creation succesfull: " << endl;
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


// ============================================================================
// ===                            Location change tools                     ===
// ============================================================================

void moveNode(int id, int x, int y) {
    shared_ptr<Node> node = objectKingdom.find(id);
    node->setX(x);
    node->setY(y);

    if (shared_ptr<Field> field = std::dynamic_pointer_cast<Field>(node)) {
        for (int j = 0; j < objectKingdom.hulls.size(); j++) {
            if (objectKingdom.rayCasting(objectKingdom.hulls[j]->points, std::make_pair(x, y))) {
                // scale of grounds here
                field->setProduction(
                    static_cast<int>(static_cast<double>(field->getProduction() * (1 + static_cast<double>(objectKingdom.hulls[j]->groundClass - 3) / 10.0)))
                );
                return;
            }
        }
        field->setProduction(field->getBasicProduction());
    }
}

// ============================================================================
// ===                            Hull tools                                ===
// ============================================================================
struct Point {
    int x;
    int y;
};

int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0)
        return 0;
    return (val > 0) ? 1 : 2;
}

// jarvis algorithm
vector<Point> convexHull(vector<Point> points, int n) {
    if (n < 3) {
        cout << "Not enough points" << endl;
        return points;
    }

    vector<Point> hull;

    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;

    int p = l, q;

    do
    {
        hull.push_back(points[p]);

        q = (p + 1) % n;
        for (int i = 0; i < n; i++)
        {
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }

        p = q;

    }
    while (p != l);

    return hull;
}

void createHull(string points, int groundClass) {
    objectKingdom.addHull(groundClass);
    HullCounter += 1;

    std::stringstream ss(points);
    std::string token;

    vector<Point> pointsV;

    while (std::getline(ss, token, ',')) {
        int x = std::stoi(token);
        std::getline(ss, token, ',');
        int y = std::stoi(token);
        pointsV.push_back({x, y});
    }

    vector<Point> hull = convexHull(pointsV, pointsV.size());
    //vector<Point> hull = pointsV;

    cout << "points: " << points << " size of hull: " << hull.size() << endl;

    for (int i = 0; i < hull.size(); i++) {
        objectKingdom.hulls[HullCounter]->points.push_back({hull[i].x, hull[i].y});
    }

    if (objectKingdom.hulls[HullCounter]->points.size() < points.size()/2) {
        cout << "not convex hull" << endl;
    }

    //objectKingdom.hulls[HullCounter]->points.push_back({x, y});
    for (int i = 0; i < objectKingdom.hulls[HullCounter]->points.size(); i++) {
        cout << objectKingdom.hulls[HullCounter]->points[i].first << " " << objectKingdom.hulls[HullCounter]->points[i].second << endl;
    }

    // set production to all fields inside a hull
    for (int i = 0; i < objectKingdom.nodeVector.size(); i += 3) {
        if (objectKingdom.rayCasting(objectKingdom.hulls[HullCounter]->points, (std::make_pair<int, int>(objectKingdom.nodeVector[i]->getX(), objectKingdom.nodeVector[i]->getY())))) {
            shared_ptr<Field> field = dynamic_pointer_cast<Field>(objectKingdom.nodeVector[i]);
            cout << field->getID() << endl;
            field->setProduction(
                static_cast<int>(static_cast<double>(field->getProduction() * (1 + static_cast<double>(objectKingdom.hulls[HullCounter]->groundClass - 3) / 10.0)))
            );
        }
    }
    //objectKingdom.addHull(groundClass);
}

void addPointToHull(int hullId, int x, int y) {
    vector<Point> points;
    for (int i = 0; i < objectKingdom.hulls[hullId]->points.size(); i++) {
        points.push_back({objectKingdom.hulls[hullId]->points[i].first, objectKingdom.hulls[hullId]->points[i].second});
    }

    points.push_back({x, y});
    vector<Point> hull = convexHull(points, points.size());

    objectKingdom.hulls[hullId]->points.clear();
    for (int i = 0; i < hull.size(); i++) {
        objectKingdom.hulls[hullId]->points.push_back({hull[i].x, hull[i].y});
    }


    // set production to all fields inside a hull
    for (int i = 0; i < objectKingdom.nodeVector.size(); i += 3) {
        if (objectKingdom.rayCasting(objectKingdom.hulls[HullCounter]->points, (std::make_pair<int, int>(objectKingdom.nodeVector[i]->getX(), objectKingdom.nodeVector[i]->getY())))) {
            shared_ptr<Field> field = dynamic_pointer_cast<Field>(objectKingdom.nodeVector[i]);
            cout << field->getID() << endl;
            field->setProduction(
                static_cast<int>(static_cast<double>(field->getProduction() * (1 + static_cast<double>(objectKingdom.hulls[HullCounter]->groundClass - 3) / 10.0)))
            );
        }
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

int isInWhichHull(int x, int y) {
    for (int i = 0; i < objectKingdom.hulls.size(); ++i) {
        cout << x << " " << y << endl;
        for (int j = 0; j < objectKingdom.hulls[i]->points.size(); j++) {
            cout << "this: " << objectKingdom.hulls[i]->points[j].first << " " << objectKingdom.hulls[i]->points[j].second << endl;
        }
        if (objectKingdom.rayCasting(objectKingdom.hulls[i]->points, std::make_pair(x, y))) {
            return i;
        }
    }
    return -1;
}

// ============================================================================
// ===                            Deletion tools                            ===
// ============================================================================
void deleteHull(int id) {
    for (int i = 0; i < objectKingdom.nodeVector.size(); i += 3) {
        if (objectKingdom.rayCasting(objectKingdom.hulls[id]->points, (std::make_pair<int, int>(objectKingdom.nodeVector[i]->getX(), objectKingdom.nodeVector[i]->getY())))) {
            shared_ptr<Field> field = dynamic_pointer_cast<Field>(objectKingdom.nodeVector[i]);
            field->setProduction(field->getBasicProduction());
        }
    }
    objectKingdom.hulls.erase(objectKingdom.hulls.begin() + id);
}

void deleteRelation(int id1, int id2) {
    shared_ptr<Node> from = objectKingdom.find(id1);
    shared_ptr<Node> to = objectKingdom.find(id2);

    for (int i = 0; i < objectKingdom.adjList[from].size(); i++)
        if (objectKingdom.adjList[from][i].getToPtr() == to) {
            objectKingdom.adjList[from].erase(objectKingdom.adjList[from].begin() + i);
            return;
        }


    for (int i = 0; i < objectKingdom.adjList[to].size(); i++)
        if (objectKingdom.adjList[to][i].getToPtr() == from) {
            objectKingdom.adjList[to].erase(objectKingdom.adjList[to].begin() + i);
            return;
        }
}

void deleteNode(int id) {
    // delete all lines from this node
    shared_ptr<Node> delNode = objectKingdom.find(id);
    objectKingdom.adjList[delNode].clear();
    cout << "node lines deleted" << endl;

    // delete all lines to this node
    for (int i = 0; i < objectKingdom.nodeVector.size(); i++) {
        for (int j = 0; j < objectKingdom.adjList[objectKingdom.nodeVector[i]].size(); j++) {
            if (objectKingdom.adjList[objectKingdom.nodeVector[i]][j].getToPtr() == delNode) {
                objectKingdom.adjList[objectKingdom.nodeVector[i]].erase(objectKingdom.adjList[objectKingdom.nodeVector[i]].begin() + j);
            }
        }
    }

    // delete node
    auto & vec = objectKingdom.nodeVector;
    vec.erase(std::remove(vec.begin(), vec.end(), delNode), vec.end());
}


string getSeed() {
    return objectKingdom.getSeed();
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("getRelationsAndCoordinates", &getRelationsAndCoordinates);
    emscripten::function("getNoRelationsCoordinates", &getNoRelationsCoordinates);
    emscripten::function("getHulls", &getHulls);
    emscripten::function("calculateFlow", &calculateFlow);

    // create object functions
    emscripten::function("createField", &createField);
    emscripten::function("createBrewery", &createBrewery);
    emscripten::function("createPub", &createPub);
    emscripten::function("createRelation", &createRelation);

    // move object functions
    emscripten::function("moveNode", &moveNode);

    // hull functions
    emscripten::function("isInWhichHull", &isInWhichHull);
    emscripten::function("createHull", &createHull);
    emscripten::function("addPointToHull", &addPointToHull);
    emscripten::register_vector<int>("VectorInt");

    // delete object functions
    emscripten::function("deleteNode", &deleteNode);
    emscripten::function("deleteHull", &deleteHull);
    emscripten::function("deleteRelation", &deleteRelation);

    // helper functions
    emscripten::function("getNodesCoordinates", &getNodesCoordinates);
    emscripten::function("isInAnyHull", &isInAnyHull);
    emscripten::function("getSeed", &getSeed);

    emscripten::function("saveToCSV", &saveToCSV);
}
