#pragma once
#include "Pub.h"
#include <vector>
#include "Intersection.h"
#include "Brewery.h"
#include "Field.h"

//chwilowo bo u mnie sie z tym nie chce kompilowac
//#include <optional>
//#include <variant>
#include <memory>
#include <map>
#include "Node.h"
#include "Lane.h"

//vektor i funkcja find od przerobienia, będziemy indeksować n + 0 brewery, n + 1 fieldy itd. po czym portować żeby usprawnić wyszukiwanie
// wyszukiwanie prawie na pewno się przyda szybkie

class Country
{
private:
	int breweryEfficiency;
public:
	std::map<std::shared_ptr<Node>, std::vector<Lane> > adjList;
	std::vector<std::shared_ptr<Node>> nodeVector;

	Country();
	~Country();

	void bfs(std::shared_ptr<Node> startingNode);
	void addRelationship(Lane lane);
	std::vector<Lane> augmentingPathBfs(std::shared_ptr<Node> source, std::shared_ptr<Node> sink, std::map<std::shared_ptr<Node>, std::vector<Lane> > adjListCopy);
	int EdmondsKarp(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	std::shared_ptr<Pub> createPub(std::string ID, int xMiiddle,int yMiddle,int radius);
	std::shared_ptr<Brewery> createBrewery(std::string ID, int xMiddle,int yMiddle,int radius,int barleyAmount);
	std::shared_ptr<Intersection> createIntersection(int x,int y);
	std::shared_ptr<Field> createField(std::string ID, int production,int xMiddle,int yMiddle,int radius);
	std::shared_ptr<Node> find(std::string ID);


	void printContent();
	void printBfs(std::shared_ptr<Node> startingNode);
};