#pragma once
#include "Pub.h"
//#include "Lane.h"
#include <vector>
#include "Intersection.h"
#include "Brewery.h"
#include "Field.h"
#include <optional>
#include <variant>
#include <memory>
#include <map>
#include "Node.h"

class Country
{
private:
	int idCounter;
	int breweryEfficiency;
	std::vector<Pub> pubs;
	std::vector<Brewery> breweries;
	std::vector<Field> fields;
	std::vector<Intersection> intersections;
	//std::vector<Lane> lanes;
	std::map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node> > > adjList;
public:
	Country();
	~Country();

	void insertPub(Pub x);
	void insertField(Field x);
	void insertBrewery(Brewery x);
	void insertIntersection(Intersection x);
	std::optional <Pub> getPub(int id);
	std::optional <Brewery> getBrewery(int id);
	std::optional <Field> getField(int id);
	std::optional <Intersection> getIntersection(int id);
	bool makeConnectionById(int id1, int id2);
	void bfs(int startingId);
	void addRelationship(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	std::shared_ptr<Pub> createPub(int xMiiddle,int yMiddle,int radius);
	std::shared_ptr<Brewery> createBrewery(int xMiddle,int yMiddle,int radius,int barleyAmount, int beerAmount);
	std::shared_ptr<Intersection> createIntersection(int x,int y,int flow);
	std::shared_ptr<Field> createField(int production,int xMiddle,int yMiddle,int radius);
};

