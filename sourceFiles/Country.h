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
	int convRate = 1;
	std::string seed = "DEFAULT";
public:
	struct CompareByX {
	    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
		return a.first < b.first;
	    }
	};
	
	struct CompareByY {
	    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
		return a.second < b.second;
	    }
	};

	std::map<std::shared_ptr<Node>, std::vector<Lane> > adjList;
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::unordered_map<std::shared_ptr<Node>, int> level;
	std::vector<std::shared_ptr<Hull>> hulls;

	Country();
	~Country();

	void setConvRate(int convRate);
	void setSeed(std::string seed);
	int getConvRate();
	std::string getSeed();

	void reset();
	void bfs(std::shared_ptr<Node> startingNode);
	void addRelationship(std::map<std::shared_ptr<Node>, std::vector<Lane> > &adjList,Lane lane);
	void addRelationship(Lane lane);
	std::vector<Lane> augmentingPathBfs(std::shared_ptr<Node> source, std::shared_ptr<Node> sink, std::map<std::shared_ptr<Node>, std::vector<Lane> > adjListCopy);
	void insertNodeSorted(std::vector<std::shared_ptr<Node>>& nodeVector, std::shared_ptr<Node> newNode);
	std::shared_ptr<Pub> createPub(int ID, int xMiiddle,int yMiddle,int radius, int capacity);
	std::shared_ptr<Brewery> createBrewery(int ID, int xMiddle,int yMiddle,int radius, int barleyCap);
	std::shared_ptr<Intersection> createIntersection(int x,int y);
	std::shared_ptr<Field> createField(int ID, int production,int xMiddle,int yMiddle,int radius);
	std::shared_ptr<Node> find(int ID);
	std::pair<int,int> edmondsKarpManyToMany(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> &toVec, int convRate);
	std::pair<int,int> dinic(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>>& toVec,int convRate);
	void printContent();
	int sendFlow(std::shared_ptr<Node> u, std::shared_ptr<Node> sink, int flow,std::map<std::shared_ptr<Node>, std::vector<Lane>>& adjListCopy, std::unordered_map<std::shared_ptr<Node>, int>& next); 
	bool buildLevelGraph(std::shared_ptr<Node> source, std::shared_ptr<Node> sink,std::map<std::shared_ptr<Node>, std::vector<Lane>>& adjListCopy);

	void addHull(int groundClass);
	std::vector<std::shared_ptr<Node>> bellmanFord(std::shared_ptr<Node> superSource,std::shared_ptr<Node> superSink,int nodeCounter, std::map<std::shared_ptr<Node>, std::vector<Lane>> adjListCopy);
	int crosses(std::pair<int,int> point,std::pair<int,int> q,std::pair<int,int> pi,std::pair<int,int> pi1,std::pair<int,int> pi2,std::pair<int,int> pi_1);
	int det(std::pair<int,int> a,std::pair<int,int> b, std::pair<int,int> c);
	bool rayCasting(std::vector<std::pair<int,int>> pointVec,std::pair<int,int> point);
	std::pair<int,std::vector<std::shared_ptr<Node>>> mcmf(std::vector<std::shared_ptr<Node>> fromVec,std::vector<std::shared_ptr<Node>> &toVec,int convRate);
	std::pair<int,int> fordFulkerson(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> &toVec,int convRate);
	std::vector<Lane> augmentingPathDFS(std::shared_ptr<Node> source, std::shared_ptr<Node> sink,std::map<std::shared_ptr<Node>, std::vector<Lane> > adjListCopy) ;
};
