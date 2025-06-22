#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../sourceFiles/Country.h"
#include "../sourceFiles/Node.h"
#include "../sourceFiles/Brewery.h"
#include "../sourceFiles/Field.h"
#include "../sourceFiles/Pub.h"
#include "../sourceFiles/Lane.h"
#include "../sourceFiles/Intersection.h"
#include "../sourceFiles/Hull.h"

#define ROW_LENGTH 12
#define CONST_RADIUS 4

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "in use: " << argv[0] << " <file path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string csvData = buffer.str();
    file.close();

    Country objectKingdom;
    std::vector<std::shared_ptr<Node>> sinks;
    std::vector<std::shared_ptr<Node>> sources;

    std::string data(csvData);
    std::stringstream ss(data);
    std::string line;
    int HullCounter = -1;
    int fieldsCounter = 0, breweriesCounter = 0, pubsCounter = 0;

    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        std::vector<std::string> row(ROW_LENGTH);

        for (int i = 0; i < ROW_LENGTH; i++) {
            std::getline(lineStream, row[i], ',');
        }

        if (row[0] == "Hull_" + std::to_string(HullCounter + 1)) {
            objectKingdom.addHull(std::stoi(row[11]));
            HullCounter++;
        } else if (row[0].find("HullPoint") != std::string::npos) {
            objectKingdom.hulls[HullCounter]->addPoint(std::make_pair(std::stoi(row[5]), std::stoi(row[6])));
        } else if (row[0] == "Field") {
            objectKingdom.createField(std::stoi(row[1]), std::stoi(row[2]), std::stoi(row[5]), std::stoi(row[6]), CONST_RADIUS);
            fieldsCounter++;
        } else if (row[0] == "Brewery") {
            objectKingdom.createBrewery(std::stoi(row[1]), std::stoi(row[5]), std::stoi(row[6]), CONST_RADIUS, std::stoi(row[3]));
            breweriesCounter++;
        } else if (row[0] == "Pub") {
            objectKingdom.createPub(std::stoi(row[1]), std::stoi(row[5]), std::stoi(row[6]), CONST_RADIUS, std::stoi(row[4]));
            pubsCounter++;
        } else if (row[0] == "Lane") {
            auto from = objectKingdom.find(std::stoi(row[7]));
            auto to = objectKingdom.find(std::stoi(row[8]));
            if (from && to) {
                Lane lane(from, to, std::stod(row[9]), std::stoi(row[10]));
                objectKingdom.addRelationship(lane);
            }
        }   else if (row[0] == "Seed") {
            objectKingdom.setSeed(row[13]);
        }   else if (row[0] == "convRate") {
            objectKingdom.setConvRate(stoi(row[12]));
        }
    }

    // Adjust production based on ground class in hulls
    for (size_t i = 0; i < objectKingdom.nodeVector.size(); i += 3) {
        for (size_t j = 0; j < objectKingdom.hulls.size(); j++) {
            if (objectKingdom.rayCasting(objectKingdom.hulls[j]->points, std::make_pair(objectKingdom.nodeVector[i]->getX(), objectKingdom.nodeVector[i]->getY()))) {
                auto derived = std::dynamic_pointer_cast<Field>(objectKingdom.nodeVector[i]);
                if (derived != nullptr) {
                    derived->setProduction(
                        static_cast<int>(
                            static_cast<double>(derived->getProduction()) * (1 + static_cast<double>(objectKingdom.hulls[j]->groundClass - 3) / 10.0)
                        )
                    );
                }
            }
        }
    }

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

	int time = 0;
    std::pair<int,int> para = objectKingdom.fordFulkerson(sources,sinks, objectKingdom.getConvRate());
	std::cout << "1 ford-fulkerson: " <<para.first<<std::endl;
    time+=para.second;
	std::cout << "1 ford-fulkerson Time: " << time << std::endl;
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

    para = objectKingdom.fordFulkerson(sources,sinks, objectKingdom.getConvRate());
    std::cout << "2 ford-fulkerson: " <<para.first<< std::endl;
    time+=para.second;
	std::cout << "2 ford-fulkerson Time: " << time << std::endl;


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
	
    para = objectKingdom.edmondsKarpManyToMany(sources,sinks, objectKingdom.getConvRate());
    std::cout << "1 Karp: " <<para.first<< std::endl;
    time+=para.second;
	std::cout << "1 Karp Time: " << time << std::endl;
    sources.clear();
    sinks.clear();
    time=0;

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

    para = objectKingdom.edmondsKarpManyToMany(sources,sinks, objectKingdom.getConvRate());
    std::cout << "2 Karp: " <<para.first<< std::endl;
    time+=para.second;
	std::cout << "2 Karp Time: " << time << std::endl;
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

    para = objectKingdom.dinic(sources,sinks, objectKingdom.getConvRate());
    std::cout << "1 dinic: " <<para.first<< std::endl;
    time+=para.second;
	std::cout << "1 dinic Time: " << time << std::endl;
    sources.clear();
    sinks.clear();
    time=0;

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

    para = objectKingdom.dinic(sources,sinks, objectKingdom.getConvRate());
    std::cout << "2 dinic: " <<para.first<< std::endl;
    time+=para.second;
	std::cout << "2 dinic Time: " << time << std::endl;
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


    return 0;
}


// g++ main.cpp ../sourceFiles/Country.cpp ../sourceFiles/Brewery.cpp ../sourceFiles/Field.cpp ../sourceFiles/Pub.cpp ../sourceFiles/Lane.cpp ../sourceFiles/Node.cpp ../sourceFiles/Hull.cpp ../sourceFiles/Intersection.cpp -o Independent_module

	/*
	Country polska;	

	std::vector<std::shared_ptr<Node>> sinks;
	std::vector<std::shared_ptr<Node>> sources;

	auto a = polska.createField(2,4, 1, 4, 4);
	a->setName("a");
	auto b = polska.createField(2,3, 1, 4, 4);
	b->setName("b");
	auto c = polska.createField(2,2, 1, 4, 4);
	c->setName("c");

	auto e = polska.createBrewery(7,1, 4, 5, 155);
	e->setName("e");
	auto f = polska.createBrewery(7,1, 4, 5, 155);
	f->setName("f");
	auto g = polska.createBrewery(7,1, 4, 5, 155);
	g->setName("g");

	auto h = polska.createPub(1,1, 2, 4, 100);
	h->setName("h");
	auto i = polska.createPub(3,2, 3, 5, 100);
	i->setName("i"); 
	
	Lane lane1(a,e ,1, 1);
	polska.addRelationship(lane1);

	Lane lane2(b, f,1,1);
	polska.addRelationship(lane2);
	
	Lane lane3(b, g, 3,1);
	polska.addRelationship(lane3); 
	
	Lane lane4(c, f, 1,4);
	polska.addRelationship(lane4); 
	
	Lane lane5(e, i, 1,1);
	polska.addRelationship(lane5); 
	
	Lane lane6(f,h,2,1);
	polska.addRelationship(lane6);

	Lane lane7(f,i, 3,1);
	polska.addRelationship(lane7);
	
	Lane lane8(g, i,4,1);
	polska.addRelationship(lane8);
	*/
//	sources.push_back(pubMentzena);
//	sinks.push_back(pubPodKogutem);

//	std::cout<<polska.dinic(sources,sinks)<<std::endl;
//	polska.cycleCancelling(sinks,sources);

//	std::cout<<polska.mcmf(sources,sinks);