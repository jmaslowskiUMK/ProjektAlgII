#pragma once
#include "Pub.h"
#include <vector>
#include "Intersection.h"
#include "Brewery.h"
#include "Field.h"
#include <optional>
#include <variant>
#include <memory>
#include <map>
#include "Node.h"
#include "Lane.h"

class Country
{
private:
	int breweryEfficiency;
	std::map<std::shared_ptr<Node>, std::vector<Lane> > adjList;
public:
	Country();
	~Country();

	void bfs(std::shared_ptr<Node> startingNode);
	void addRelationship(Lane lane);
	std::vector<Lane> augmentingPathBfs(std::shared_ptr<Node> source, std::shared_ptr<Node> sink, std::map<std::shared_ptr<Node>, std::vector<Lane> > adjListCopy);
	int EdmondsKarp(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	std::shared_ptr<Pub> createPub(int xMiiddle,int yMiddle,int radius);
	std::shared_ptr<Brewery> createBrewery(int xMiddle,int yMiddle,int radius,int barleyAmount);
	std::shared_ptr<Intersection> createIntersection(int x,int y);
	std::shared_ptr<Field> createField(int production,int xMiddle,int yMiddle,int radius);
};

