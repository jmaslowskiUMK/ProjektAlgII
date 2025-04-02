#include "Country.h"
#include <memory>
#include <map>
#include "Node.h"

Country::Country() {
	idCounter = 0;
}

Country::~Country() {
	pubs.clear();
	//lanes.clear();
	fields.clear();
	breweries.clear();
	intersections.clear();
}

/*void Country::insertPub(Pub x) {
	pubs.push_back(x);
	idCounter++;
	std::vector<int> dummy;
	adjacencyList.push_back(dummy);
}

void Country::insertBrewery(Brewery x) {
	breweries.push_back(x);
	idCounter++;
	std::vector<int> dummy;
	adjacencyList.push_back(dummy);
}

void Country::insertField(Field x) {
	fields.push_back(x);
	idCounter++;
	std::vector<int> dummy;
	adjacencyList.push_back(dummy);
}

void Country::insertIntersection(Intersection x) {
	intersections.push_back(x);
	idCounter++;
	std::vector<int> dummy;
	adjacencyList.push_back(dummy);
}
*/
/*std::optional<Pub> Country::getPub(int id) {
	for (auto el : pubs) {
		if (el.getId() == id) {
			return el;
		}
	}
	return {};
}

std::optional <Brewery> Country::getBrewery(int id) {
	for (auto el : breweries) {
		if (el.getId() == id) {
			return el;
		}
	}
	return {};
}

std::optional <Field> Country::getField(int id) {
	for (auto el : fields) {
		if (el.getId() == id) {
			return el;
		}
	}
	return {};
}

std::optional <Intersection> Country::getIntersection(int id) {
	for (auto el : intersections) {
		if (el.getId() == id) {
			return el;
		}
	}
	return {};
}
*/

/*bool Country::makeConnectionById(int id1, int id2) {
	if (id1 > adjacencyList.size() || id2 > adjacencyList.size())return false;
	adjacencyList[id1].push_back(id2);
	adjacencyList[id2].push_back(id1);
}
*/
void Country::bfs(int startingId) {

}

void Country::addRelationship(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
	adjList[from].emplace_back(to);
}

std::shared_ptr<Pub> Country::createPub(int xMiiddle, int yMiddle, int radius){
	auto pub = std::make_shared<Pub>(xMiiddle,yMiddle,radius);
	adjList[pub] = {};
	return pub;
}

std::shared_ptr<Brewery> Country::createBrewery(int xMiddle, int yMiddle, int radius, int barleyAmount, int beerAmount) {
	auto brewery = std::make_shared<Brewery>(xMiddle, yMiddle, radius, barleyAmount, beerAmount);
	adjList[brewery] = {};
	return brewery;
}

std::shared_ptr<Intersection> Country::createIntersection(int x, int y, int flow) {
	auto intersection = std::make_shared<Intersection>(x, y, flow);
	adjList[intersection] = {};
	return intersection;
}

std::shared_ptr<Field> Country::createField(int production, int xMiddle, int yMiddle, int radius){
	auto field = std::make_shared<Field>(production, xMiddle, yMiddle, radius);
	adjList[field] = {};
	return field;
}
