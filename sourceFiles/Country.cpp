#include "Country.h"
#include <memory>
#include <map>
#include "Node.h"
#include <vector>
#include <queue>
#include <algorithm> //find
#include <limits.h>
#include <unordered_map>
#include <unordered_set>

//dodałem bo ostatnia funkcja
#include <iostream>

Country::Country() {
}

Country::~Country() {
	
}

void Country::bfs(std::shared_ptr<Node> startingNode) {
	std::vector<std::shared_ptr<Node> > visited;
	std::queue<std::shared_ptr<Node>> q;
	q.push(startingNode);
	std::shared_ptr<Node> curr;

	while (!q.empty()) {
		curr = q.front();//take the first element from queue
		visited.push_back(q.front());
		for (auto el : adjList[curr]) {//for every EDGE that curr VERTIX points to
			if (std::find(visited.begin(), visited.end(), el.getToPtr()) == visited.end()) {//visited.begin if el is found, viisited.end otherwise
				q.push(el.getToPtr());
			}
		}
		curr.get()->printName();
		q.pop();//delete the EDGE after pushing all its neighbours to the queue
	}
}

std::vector<Lane> Country::augmentingPathBfs(std::shared_ptr<Node> source, std::shared_ptr<Node> sink,std::map<std::shared_ptr<Node>, std::vector<Lane> > adjListCopy) {
	std::queue<std::shared_ptr<Node>> q;
	std::unordered_map<std::shared_ptr<Node>, Lane> parent;
	std::unordered_set<std::shared_ptr<Node>> visited;

	q.push(source);
	visited.insert(source);

	while (!q.empty()) {
		auto curr = q.front();//take the first element from queue
		q.pop();//pop the element that is currently held in curr 

		for (auto& neighbor : adjListCopy[curr]) {//get EDGES from current VERTIX 
			auto next = neighbor.getToPtr();
			if (visited.count(next) == 0 && neighbor.getFlow() > 0) {//check if we have already viseted and if the flow is >0
				parent[next] = neighbor;
				visited.insert(next);
				q.push(next);

				if (next == sink) {//check if finished
					std::vector<Lane> path;//returning vector
					for (auto node = sink; node != source; node = parent[node].getFromPtr()) {//create a path from parent
						path.push_back(parent[node]);
					}
					std::reverse(path.begin(), path.end());
					return path;
				}
			}
		}
	}
	return {};
}


int Country::edmondsKarp(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
	int max_flow = 0;
	std::map<std::shared_ptr<Node>, std::vector<Lane> > adjListCopy = adjList;// create a copy of adjList, so that adjList stays the same after adding reverse edges
	while (true) {
		std::vector<Lane> path = augmentingPathBfs(from, to, adjListCopy);//look for aumenting path
		if (path.empty()) break;//if there is no way from source to sink we are finished

		int minFlow = INT_MAX;//limits.h
		for (auto& lane : path) {//looking for minFlow in our augmenting path as it is the maximum that can flow throughh that path
			if (lane.getFlow() < minFlow) {
				minFlow = lane.getFlow();
			}
		}

		for (auto& lane : path) {//add reverseFlows
			for (auto& l : adjListCopy[lane.getFromPtr()]) {//substract flow from aumenting path
				if (l.getToPtr() == lane.getToPtr()) {
					l.setFlow(l.getFlow() - minFlow);
					break;
				}
			}

			bool foundReverse = false;
			for (auto& l : adjListCopy[lane.getToPtr()]) {
				if (l.getToPtr() == lane.getFromPtr()) {
					l.setFlow(l.getFlow() + minFlow);
					foundReverse = true;
					break;
				}
			}
			if (!foundReverse) {//create reverse Paths
				adjListCopy[lane.getToPtr()].emplace_back(lane.getToPtr(), lane.getFromPtr(), minFlow);
			}
		}

		max_flow += minFlow;// add flow from each augmenting path to resulting max flow
	}

	return max_flow;
}


void Country::addRelationship(Lane lane) {//add Lane to adjList
	adjList[lane.getFromPtr()].push_back(lane);
}

void Country::addRelationship(	std::map<std::shared_ptr<Node>, std::vector<Lane> > &list, Lane lane) {//add Lane to adjList
	list[lane.getFromPtr()].push_back(lane);
}


//create shared_ptr of each derived from Node class add to adjList

std::shared_ptr<Pub> Country::createPub(std::string ID, int xMiiddle, int yMiddle, int radius){
	auto pub = std::make_shared<Pub>(ID, xMiiddle,yMiddle,radius);
	adjList[pub] = {};
	this->nodeVector.push_back(pub);
	return pub;
}

std::shared_ptr<Brewery> Country::createBrewery(std::string ID, int xMiddle, int yMiddle, int radius, int barleyAmount) {
	auto brewery = std::make_shared<Brewery>(ID, xMiddle, yMiddle, radius, barleyAmount);
	adjList[brewery] = {};
	nodeVector.push_back(brewery);
	return brewery;
}

std::shared_ptr<Intersection> Country::createIntersection(int x, int y) {
	auto intersection = std::make_shared<Intersection>(x, y);
	adjList[intersection] = {};
	nodeVector.push_back(intersection);
	return intersection;
}

std::shared_ptr<Field> Country::createField(std::string ID, int production, int xMiddle, int yMiddle, int radius){
	auto field = std::make_shared<Field>(ID, production, xMiddle, yMiddle, radius);
	adjList[field] = {};
	nodeVector.push_back(field);
	return field;
}

std::shared_ptr<Node> Country::find(std::string ID) {
	for (int i = 0; i < nodeVector.size(); i++) {
		if (ID == nodeVector[i]->getID()) {
			return nodeVector[i];
		}
	}
	return std::shared_ptr<Node>();
}

void Country::printContent() {
	for (int i = 0; i < nodeVector.size(); i++) {
		nodeVector[i]->printName();
	}
}

void Country::printBfs(std::shared_ptr<Node> startingNode) {
	std::vector<std::shared_ptr<Node>> visited;
    std::queue<std::shared_ptr<Node>> q;
    q.push(startingNode);
    std::shared_ptr<Node> curr;

    while (!q.empty()) {
        curr = q.front();  // Pobierz pierwszy element z kolejki
        visited.push_back(curr);
        q.pop();  // Usuwamy wierzchołek z kolejki po przetworzeniu

        // Iteruj po krawędziach (relacjach) wierzchołka
        for (auto& el : adjList[curr]) {
            auto toNode = el.getToPtr();  // Wierzchołek docelowy
            if (std::find(visited.begin(), visited.end(), toNode) == visited.end()) {  // Jeśli nie odwiedzony
                q.push(toNode);  // Dodaj do kolejki
            }

            // Wyświetl relację w formie strzałki
            curr->printName();  // Wypisz nazwę wierzchołka początkowego
            std::cout << " -> "; // Strzałka
            toNode->printName(); // Wypisz nazwę wierzchołka docelowego
            std::cout << std::endl;
        }
    }
}

int Country::edmondsKarpManyToMany(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> toVec) {
    int max_flow = 0;
    std::map<std::shared_ptr<Node>, std::vector<Lane>> adjListCopy = adjList;// create a copy of adjList, so that adjList stays the same after adding reverse edges


	//create superSink and superSource that connect all the sinks and souerces as one superSink and superSource. THEY ARE CONNECTED BY EGDES WITH INF FLOW.
    auto superSource = std::make_shared<Field>(); 
    auto superSink = std::make_shared<Pub>();    

    for (auto& source : fromVec) {//add to adjListCopy edges from superSource to all the sources
        Lane lane(superSource, source, INT_MAX);
        addRelationship(adjListCopy, lane);
    }

    for (auto& sink : toVec) {//add to adjListCopy edges from sinks to superSink
        Lane lane(sink, superSink, INT_MAX);
        addRelationship(adjListCopy, lane);
    }

    while (true) {
        std::vector<Lane> path = augmentingPathBfs(superSource, superSink, adjListCopy);
        if (path.empty()) break;//if there is no way from source to sink we are finished


        int minFlow = INT_MAX;//limits.h
        for (auto& lane : path) {//looking for minFlow in our augmenting path as it is the maximum that can flow throughh that path
            minFlow = std::min(minFlow, lane.getFlow());
        }

        //update flows and add reverse edges
        for (auto& lane : path) {
            bool updated = false;
            auto& lanes = adjListCopy[lane.getFromPtr()];
            for (auto& l : lanes) {//substract flow from aumenting path
                if (l.getToPtr() == lane.getToPtr()) {
                    l.setFlow(l.getFlow() - minFlow);
                    updated = true;
                    break;
                }
            }

            if (!updated) {
                continue;
            }

            bool foundReverse = false;
            auto& revLanes = adjListCopy[lane.getToPtr()];
            for (auto& l : revLanes) {
                if (l.getToPtr() == lane.getFromPtr()) {
                    l.setFlow(l.getFlow() + minFlow);
                    foundReverse = true;
                    break;
                }
            }
            if (!foundReverse) {//create reverse Paths
                revLanes.emplace_back(lane.getToPtr(), lane.getFromPtr(), minFlow);
            }
        }

        max_flow += minFlow;
    }

    return max_flow;// add flow from each augmenting path to resulting max flow
}


int Country::dinic(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> toVec){

	int max_flow = 0;
	std::map<std::shared_ptr<Node>, std::vector<Lane>> adjListCopy = adjList;// create a copy of adjList, so that adjList stays the same after adding reverse edges

	//create superSink and superSource that connect all the sinks and souerces as one superSink and superSource. THEY ARE CONNECTED BY EGDES WITH INF FLOW.
    auto superSource = std::make_shared<Field>(); 
    auto superSink = std::make_shared<Pub>();    

    for (auto& source : fromVec) {//add to adjListCopy edges from superSource to all the sources
        Lane lane(superSource, source, INT_MAX);
        addRelationship(adjListCopy, lane);
    }

    for (auto& sink : toVec) {//add to adjListCopy edges from sinks to superSink
        Lane lane(sink, superSink, INT_MAX);
        addRelationship(adjListCopy, lane);
    }

	
	while (buildLevelGraph(superSource, superSink, adjListCopy)) {
        std::unordered_map<std::shared_ptr<Node>, int> next; // indeks, który śledzi "które krawędzie już próbowałem"
        int flow;
        while ((flow = sendFlow(superSource, superSink, INT_MAX, adjListCopy, next)) > 0) {
            max_flow += flow;
        }
    }
	return max_flow;

}


bool Country::buildLevelGraph(std::shared_ptr<Node> source, std::shared_ptr<Node> sink,std::map<std::shared_ptr<Node>, std::vector<Lane>>& adjListCopy) {

    level.clear();
    std::queue<std::shared_ptr<Node>> q;

    q.push(source);
    level[source] = 0;

    while (!q.empty()) {
        auto u = q.front();
        q.pop();

        for ( auto& lane : adjListCopy[u]) {
            auto v = lane.getToPtr();
            if (level.find(v) == level.end() && lane.getFlow() > 0) {
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }

    return level.find(sink) != level.end();
}

int Country::sendFlow(std::shared_ptr<Node> u,std::shared_ptr<Node> sink, int flow,std::map<std::shared_ptr<Node>, std::vector<Lane>>& adjListCopy, std::unordered_map<std::shared_ptr<Node>, int>& next) {

    if (u == sink) return flow;

    for (int& i = next[u]; i < adjListCopy[u].size(); i++) {
        Lane& lane = adjListCopy[u][i];
        auto v = lane.getToPtr();

        if (lane.getFlow() > 0 && level[v] == level[u] + 1) {
            int currFlow = std::min(flow, lane.getFlow());
            int tempFlow = sendFlow(v, sink, currFlow, adjListCopy, next);

            if (tempFlow > 0) {
                lane.setFlow(lane.getFlow() - tempFlow);

                // dodaj/przywróć odwrotną krawędź
                bool reverseFound = false;
                for (auto& revLane : adjListCopy[v]) {
                    if (revLane.getToPtr() == u) {
                        revLane.setFlow(revLane.getFlow() + tempFlow);
                        reverseFound = true;
                        break;
                    }
                }
                if (!reverseFound) {
                    adjListCopy[v].emplace_back(v, u, tempFlow);
                }

                return tempFlow;
            }
        }
    }

    return 0;
}

