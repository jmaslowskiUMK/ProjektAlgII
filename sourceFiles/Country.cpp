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


int Country::EdmondsKarp(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
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