#pragma once
#include "Pub.h"
#include <vector>
#include "Intersection.h"
#include "Brewery.h"
#include "Field.h"
//#include <optional>
//#include <variant>
#include <memory>
#include <map>
#include "Node.h"
#include "Lane.h"
#include <unordered_map>
#include "Hull.h"

class Country
{
private:
	int breweryEfficiency;
public:
	std::map<std::shared_ptr<Node>, std::vector<Lane> > adjList;
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::unordered_map<std::shared_ptr<Node>, int> level;
	std::vector<Hull> hulls;
public:

	Country();
	~Country();

	void bfs(std::shared_ptr<Node> startingNode);
	void addRelationship(std::map<std::shared_ptr<Node>, std::vector<Lane> > &adjList,Lane lane);
	void addRelationship(Lane lane);
	std::vector<Lane> augmentingPathBfs(std::shared_ptr<Node> source, std::shared_ptr<Node> sink, std::map<std::shared_ptr<Node>, std::vector<Lane> > adjListCopy);
	int edmondsKarp(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	std::shared_ptr<Pub> createPub(int ID, int xMiiddle,int yMiddle,int radius);
	std::shared_ptr<Brewery> createBrewery(int ID, int xMiddle,int yMiddle,int radius,int barleyAmount);
	std::shared_ptr<Intersection> createIntersection(int x,int y);
	std::shared_ptr<Field> createField(int ID, int production,int xMiddle,int yMiddle,int radius);
	std::shared_ptr<Node> find(int ID);
	int edmondsKarpManyToMany(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> toVec); 
	int dinic(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> toVec);
	void printContent();
	// it is useless now
	//void printBfs(std::shared_ptr<Node> startingNode);
	int sendFlow(std::shared_ptr<Node> u, std::shared_ptr<Node> sink, int flow,std::map<std::shared_ptr<Node>, std::vector<Lane>>& adjListCopy, std::unordered_map<std::shared_ptr<Node>, int>& next); 
	bool buildLevelGraph(std::shared_ptr<Node> source, std::shared_ptr<Node> sink,std::map<std::shared_ptr<Node>, std::vector<Lane>>& adjListCopy); 

};
