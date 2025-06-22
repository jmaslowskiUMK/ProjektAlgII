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
#include <iostream>
#include <stack>
#include <chrono>

Country::Country() {}
Country::~Country() {}
void Country::reset() {
    adjList.clear();
    level.clear();
    hulls.clear();
    nodeVector.clear();
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

        for (auto& neighbour : adjListCopy[curr]) {//get EDGES from current VERTIX
            auto next = neighbour.getToPtr();
            if (visited.count(next) == 0 && neighbour.getCapacity() - neighbour.getFlow() > 0) {//check if we have already viseted and if the flow is >0
                parent[next] = neighbour;
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


std::vector<Lane> Country::augmentingPathDFS(std::shared_ptr<Node> source, std::shared_ptr<Node> sink,std::map<std::shared_ptr<Node>, std::vector<Lane> > adjListCopy) {
    std::stack<std::shared_ptr<Node>> s;
    std::unordered_map<std::shared_ptr<Node>, Lane> parent;
    std::unordered_set<std::shared_ptr<Node>> visited;

    s.push(source);
    visited.insert(source);
    
    while (!s.empty()) {
        auto curr = s.top();//take the first element from queue
        s.pop();//pop the element that is currently held in curr

        for (auto& neighbour : adjListCopy[curr]) {//get EDGES from current VERTIX
            auto next = neighbour.getToPtr();
            if (visited.count(next) == 0 && neighbour.getCapacity() - neighbour.getFlow() > 0) {//check if we have already viseted and if the flow is >0
                parent[next] = neighbour;
                visited.insert(next);
                s.push(next);

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


std::pair<int,int> Country::fordFulkerson(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> &toVec,int convRate){
    auto old  = std::chrono::steady_clock::now();

    int max_flow = 0;
    std::map<std::shared_ptr<Node>, std::vector<Lane>> adjListCopy = adjList;// create a copy of adjList, so that adjList stays the same after adding reverse edges

    std::shared_ptr<Node> superSource;
    std::shared_ptr<Node> superSink;

    //create superSink and superSource that connect all the sinks and souerces as one superSink and superSource. THEY ARE CONNECTED BY EGDES WITH INF FLOW.
    auto type = std::dynamic_pointer_cast<Field>(fromVec[0]);

    if(type){
        superSource = std::make_shared<Field>();
        superSink = std::make_shared<Brewery>();

        for (auto& source : fromVec) {//add to adjListCopy edges from superSource to all the sources
            auto sourceField = std::dynamic_pointer_cast<Field>(source);
            if(sourceField){
                Lane lane(superSource, sourceField, sourceField->getProduction(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

        for (auto& sink : toVec) {//add to adjListCopy edges from sinks to superSink
            auto sinkBrew = std::dynamic_pointer_cast<Brewery>(sink);
            if(sinkBrew){
                Lane lane(sinkBrew, superSink, sinkBrew->getBarleyCap(), 0);
                addRelationship(adjListCopy, lane);
            }
        }
    }else{
        superSource = std::make_shared<Brewery>();
        superSink = std::make_shared<Pub>();

        for (auto& source : fromVec) {//add to adjListCopy edges from superSource to all the sources
            auto sourceBrew = std::dynamic_pointer_cast<Brewery>(source);
            if(sourceBrew){
                Lane lane(superSource,sourceBrew, sourceBrew->getBeerAmount(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

        for (auto& sink : toVec) {//add to adjListCopy edges from sinks to superSink
        auto sinkPub = std::dynamic_pointer_cast<Pub>(sink);
            if(sinkPub){
                 Lane lane(sinkPub, superSink, sinkPub->getCapacity(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

    } 

    while (true) {
    std::vector<Lane> path = augmentingPathBfs(superSource, superSink, adjListCopy);
    if (path.empty()) break;//if there is no way from source to sink we are finished


    int minFlow = INT_MAX;//limits.h
        for (auto& lane : path) {//looking for minFlow in our augmenting path as it is the maximum that can flow through that path
            if (lane.getCapacity() - lane.getFlow() < minFlow) {
                minFlow = lane.getCapacity() - lane.getFlow();
            }
        }

    //AUGMENT
        for (auto& lane : path) {//add return edges and substract flow from path found by augmentingPathBfs
            for (auto& l : adjListCopy[lane.getFromPtr()]) {//substract flow from aumenting path
                if (l.getToPtr() == lane.getToPtr()) {
                    l.setFlow(l.getFlow() + minFlow);
                    break;
                }
            }

            bool foundReverse = false;
            for (auto& l : adjListCopy[lane.getToPtr()]) {
                if (l.getToPtr() == lane.getFromPtr()) {
                    l.setFlow(l.getFlow() - minFlow);
                    foundReverse = true;
                    break;
                }
            }
            if (!foundReverse) {//create reverse Paths
                Lane auxLane(lane.getToPtr(), lane.getFromPtr(), (-1*minFlow) ,lane.getRepairCost());
                adjListCopy[ lane.getToPtr() ].push_back(auxLane);
            }
        }

        max_flow += minFlow;// add flow from each augmenting path to resulting max flow
    }

    if(type){
        for(auto &el:toVec){
            for(auto &el2:adjListCopy[el]){
                if(el2.getToPtr().get() == superSink.get()){
                    auto brew = std::dynamic_pointer_cast<Brewery>(el);
                    brew->setBarley(el2.getFlow());
                    brew->conversion(convRate);
                    break;
                }
            }
        } 
    }

    auto dur = std::chrono::steady_clock::now() - old; 
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
    int ms_int = static_cast<int>(ms.count()); 
    return {max_flow,ms_int};
}


std::pair<int,int> Country::edmondsKarpManyToMany(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> &toVec,int convRate){
    auto old  = std::chrono::steady_clock::now();
    int max_flow = 0;
    std::map<std::shared_ptr<Node>, std::vector<Lane>> adjListCopy = adjList;// create a copy of adjList, so that adjList stays the same after adding reverse edges

    std::shared_ptr<Node> superSource;
    std::shared_ptr<Node> superSink;

    //create superSink and superSource that connect all the sinks and souerces as one superSink and superSource. THEY ARE CONNECTED BY EGDES WITH INF FLOW.
    auto type = std::dynamic_pointer_cast<Field>(fromVec[0]);

    if(type){
        superSource = std::make_shared<Field>();
        superSink = std::make_shared<Brewery>();

        for (auto& source : fromVec) {//add to adjListCopy edges from superSource to all the sources
            auto sourceField = std::dynamic_pointer_cast<Field>(source);
            if(sourceField){
                Lane lane(superSource, sourceField, sourceField->getProduction(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

        for (auto& sink : toVec) {//add to adjListCopy edges from sinks to superSink
            auto sinkBrew = std::dynamic_pointer_cast<Brewery>(sink);
            if(sinkBrew){
                Lane lane(sinkBrew, superSink, sinkBrew->getBarleyCap(), 0);
                addRelationship(adjListCopy, lane);
            }
        }
    }else{
        superSource = std::make_shared<Brewery>();
        superSink = std::make_shared<Pub>();

        for (auto& source : fromVec) {//add to adjListCopy edges from superSource to all the sources
            auto sourceBrew = std::dynamic_pointer_cast<Brewery>(source);
            if(sourceBrew){
                Lane lane(superSource,sourceBrew, sourceBrew->getBeerAmount(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

        for (auto& sink : toVec) {//add to adjListCopy edges from sinks to superSink
        auto sinkPub = std::dynamic_pointer_cast<Pub>(sink);
            if(sinkPub){
                 Lane lane(sinkPub, superSink, sinkPub->getCapacity(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

    } 

    while (true) {
    std::vector<Lane> path = augmentingPathBfs(superSource, superSink, adjListCopy);
    if (path.empty()) break;//if there is no way from source to sink we are finished


    int minFlow = INT_MAX;//limits.h
        for (auto& lane : path) {//looking for minFlow in our augmenting path as it is the maximum that can flow through that path
            if (lane.getCapacity() - lane.getFlow() < minFlow) {
                minFlow = lane.getCapacity() - lane.getFlow();
            }
        }

    //AUGMENT
        for (auto& lane : path) {//add return edges and substract flow from path found by augmentingPathBfs
            for (auto& l : adjListCopy[lane.getFromPtr()]) {//substract flow from aumenting path
                if (l.getToPtr() == lane.getToPtr()) {
                    l.setFlow(l.getFlow() + minFlow);
                    break;
                }
            }

            bool foundReverse = false;
            for (auto& l : adjListCopy[lane.getToPtr()]) {
                if (l.getToPtr() == lane.getFromPtr()) {
                    l.setFlow(l.getFlow() - minFlow);
                    foundReverse = true;
                    break;
                }
            }
            if (!foundReverse) {//create reverse Paths
                Lane auxLane(lane.getToPtr(), lane.getFromPtr(), (-1*minFlow) ,lane.getRepairCost());
                adjListCopy[ lane.getToPtr() ].push_back(auxLane);
            }
        }

        max_flow += minFlow;// add flow from each augmenting path to resulting max flow
    }

    if(type){
        for(auto &el:toVec){
            for(auto &el2:adjListCopy[el]){
                if(el2.getToPtr().get() == superSink.get()){
                    auto brew = std::dynamic_pointer_cast<Brewery>(el);
                    brew->setBarley(el2.getFlow());
                    brew->conversion(convRate);
                    break;
                }
            }
        } 
    }

    auto dur = std::chrono::steady_clock::now() - old; 
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
    int ms_int = static_cast<int>(ms.count()); 
    return {max_flow,ms_int};
}


void Country::setConvRate(int convRate) {
    this->convRate = convRate;
}
void Country::setSeed(std::string seed) {
    this->seed = seed;
}
int Country::getConvRate() {
    return convRate;
}
std::string Country::getSeed() {
    return seed;
}

void Country::insertNodeSorted(std::vector<std::shared_ptr<Node>>& nodeVector, std::shared_ptr<Node> newNode) {
    auto it = std::lower_bound(nodeVector.begin(), nodeVector.end(), newNode,
        [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return a->getID() < b->getID();
        });
    nodeVector.insert(it, newNode);
}


void Country::addRelationship(Lane lane) {//add Lane to adjList
    adjList[lane.getFromPtr()].push_back(lane);
}

void Country::addRelationship(  std::map<std::shared_ptr<Node>, std::vector<Lane> > &list, Lane lane) {//add Lane to adjList
    list[lane.getFromPtr()].push_back(lane);
}


//create shared_ptr of each derived from Node class add to adjList

std::shared_ptr<Pub> Country::createPub(int ID, int xMiiddle, int yMiddle, int radius, int capacity){
    auto pub = std::make_shared<Pub>(ID, xMiiddle,yMiddle,radius, capacity);
    adjList[pub] = {};
    insertNodeSorted(nodeVector, pub);
    return pub;
}

std::shared_ptr<Brewery> Country::createBrewery(int ID, int xMiddle, int yMiddle, int radius, int barleyCap) {
    auto brewery = std::make_shared<Brewery>(ID, xMiddle, yMiddle, radius, barleyCap);
    adjList[brewery] = {};
    insertNodeSorted(nodeVector, brewery);
    return brewery;
}

std::shared_ptr<Intersection> Country::createIntersection(int x, int y) {
    auto intersection = std::make_shared<Intersection>(x, y);
    adjList[intersection] = {};
    insertNodeSorted(nodeVector, intersection);
    return intersection;
}

std::shared_ptr<Field> Country::createField(int ID, int production, int xMiddle, int yMiddle, int radius){
    auto field = std::make_shared<Field>(ID, production, xMiddle, yMiddle, radius);
    adjList[field] = {};
    insertNodeSorted(nodeVector, field);
    return field;
}

std::shared_ptr<Node> Country::find(int ID) {
    /*
    for (int i = 0; i < nodeVector.size(); i++) {
        std::cout << nodeVector[i]->getID() << std::endl;
    }
    for (int i = 0; i < nodeVector.size(); i++) {
        if (ID == nodeVector[i]->getID()) {
            return nodeVector[i];
        }
    }
    return std::shared_ptr<Node>();
    */
    try {
        auto it = std::lower_bound(nodeVector.begin(), nodeVector.end(), ID,
            [](const std::shared_ptr<Node>& node, int value) {
                return node->getID() < value;
        });

        if (it != nodeVector.end() && (*it)->getID() == ID) {
            return *it;
        }
    }   catch (const std::exception& e) {
        std::cout << "COS W FUNKCJI FIND" << std::endl;
    }

    return std::shared_ptr<Node>();  // nullptr
}

void Country::printContent() {
    for (int i = 0; i < nodeVector.size(); i++) {
        nodeVector[i]->printName();
    }
}

std::pair<int,int> Country::dinic(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>>& toVec,int convRate){
    auto old  = std::chrono::steady_clock::now();
 int max_flow = 0;
    std::map<std::shared_ptr<Node>, std::vector<Lane>> adjListCopy = adjList;// create a copy of adjList, so that adjList stays the same after adding reverse edges

    std::shared_ptr<Node> superSource;
    std::shared_ptr<Node> superSink;

    //create superSink and superSource that connect all the sinks and souerces as one superSink and superSource. THEY ARE CONNECTED BY EGDES WITH INF FLOW.
    auto type = std::dynamic_pointer_cast<Field>(fromVec[0]);

    if(type){
        superSource = std::make_shared<Field>();
        superSink = std::make_shared<Brewery>();

        for (auto& source : fromVec) {//add to adjListCopy edges from superSource to all the sources
            auto sourceField = std::dynamic_pointer_cast<Field>(source);
            if(sourceField){
                Lane lane(superSource, sourceField, sourceField->getProduction(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

        for (auto& sink : toVec) {//add to adjListCopy edges from sinks to superSink
            auto sinkBrew = std::dynamic_pointer_cast<Brewery>(sink);
            if(sinkBrew){
                Lane lane(sinkBrew, superSink, sinkBrew->getBarleyCap(), 0);
                addRelationship(adjListCopy, lane);
            }
        }
    }else{
        superSource = std::make_shared<Brewery>();
        superSink = std::make_shared<Pub>();

        for (auto& source : fromVec) {//add to adjListCopy edges from superSource to all the sources
            auto sourceBrew = std::dynamic_pointer_cast<Brewery>(source);
            if(sourceBrew){
                Lane lane(superSource,sourceBrew, sourceBrew->getBeerAmount(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

        for (auto& sink : toVec) {//add to adjListCopy edges from sinks to superSink
        auto sinkPub = std::dynamic_pointer_cast<Pub>(sink);
            if(sinkPub){
                 Lane lane(sinkPub, superSink, sinkPub->getCapacity(), 0);
                addRelationship(adjListCopy, lane);
            }
        }

    } 
    
    while (buildLevelGraph(superSource, superSink, adjListCopy)) {//we are building level graph similarly as we were doing with augmentingPathBfs function. Serves simmilar role
        std::unordered_map<std::shared_ptr<Node>, int> next; // TODO
        int flow;
        while ((flow = sendFlow(superSource, superSink, INT_MAX, adjListCopy, next)) > 0) {
            max_flow += flow;
        }
    }
    
    if(type){
        for(auto &el:toVec){
            for(auto &el2:adjListCopy[el]){
                if(el2.getToPtr().get() == superSink.get()){
                    auto brew = std::dynamic_pointer_cast<Brewery>(el);
                    brew->setBarley(el2.getFlow());
                    brew->conversion(convRate);
                    break;
                }
            }
        } 
    }


    auto dur = std::chrono::steady_clock::now() - old; 
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
    int ms_int = static_cast<int>(ms.count()); 
    return {max_flow,ms_int};

}


bool Country::buildLevelGraph(std::shared_ptr<Node> source, std::shared_ptr<Node> sink,std::map<std::shared_ptr<Node>, std::vector<Lane>>& adjListCopy) {

    //this function works simmilarly to bfs but it also completes std::unordered_map<std::shared_ptr<Node>, int> level;

    level.clear();
    std::queue<std::shared_ptr<Node>> q;

    q.push(source);
    level[source] = 0;

    while (!q.empty()) {//BFS-alike
        auto u = q.front();
        q.pop();

        for ( auto& lane : adjListCopy[u]) {
            auto v = lane.getToPtr();
            if (level.find(v) == level.end() && lane.getCapacity() - lane.getFlow() > 0) {
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }

    return level.find(sink) != level.end();
}

int Country::sendFlow(std::shared_ptr<Node> source,std::shared_ptr<Node> sink, int flow,std::map<std::shared_ptr<Node>, std::vector<Lane>>& adjListCopy, std::unordered_map<std::shared_ptr<Node>, int>& next) {

    if ( source == sink) return flow;

    for (int& i = next[source]; i < adjListCopy[source].size(); i++) {
        Lane& lane = adjListCopy[source][i];
        auto v = lane.getToPtr();

        if (lane.getCapacity() - lane.getFlow() > 0 && level[v] == level[source] + 1) {
            int currFlow = std::min(flow, lane.getCapacity() - lane.getFlow());
            int tempFlow = sendFlow(v, sink, currFlow, adjListCopy, next);

            if (tempFlow > 0) {
            lane.setFlow(lane.getFlow() + tempFlow);

            
            bool reverseFound = false;
            for (auto& revLane : adjListCopy[v]) {
                if (revLane.getToPtr() == source) {
                revLane.setFlow(revLane.getFlow() - tempFlow);
                reverseFound = true;
                break;
                }
            }
            if (!reverseFound) {
                adjListCopy[v].emplace_back(v, source, (-1*tempFlow), lane.getRepairCost());
            }

            return tempFlow;
            }
        }
    }

    return 0;
}

void Country::addHull(int groundClass) {
    hulls.push_back(std::make_shared<Hull>(groundClass));
}

int Country::det(std::pair<int,int> a,std::pair<int,int> b, std::pair<int,int> c){
    int result = (b.first-a.first)*(c.second-a.second) - (c.first-a.first)*(b.second-a.second);
    return result;
}

int Country::crosses(std::pair<int,int> point,std::pair<int,int> q,std::pair<int,int> pi,std::pair<int,int> pi1,
                     std::pair<int,int> pi2,std::pair<int,int> pi_1){

    ////////////////////////////////for D=det(a,b,c) if D>0 c is on the left hand-side of vec(a,b)////////////////////////////////////////////
    ////////////////////////////////for D=det(a,b,c) if D<0 c is on the right hand-side of vec(a,b)///////////////////////////////////////////
    ////////////////////////////////for D=det(a,b,c) if D==0 c is on the same line as vec(a,b)////////////////////////////////////////////////

    int d1 = det(point,q,pi);
    int d2 = det(point,q,pi1);
    int d3 = det(pi,pi1,point);
    int d4 = det(pi,pi1,q);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // check if the lines cross
    
    if(((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && 
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return 1;
    }

    if(d3==0){// check if THE point is in the edge
        //case 0
        if(std::min(pi.first, pi1.first) <= point.first && 
            point.first<=std::max(pi.first, pi1.first) 
            && std::min(pi.second, pi1.second) <= point.second 
            && point.second<=std::max(pi.second, pi1.second)  )
        {
            return 2;//2 is returned because in the rayCasting func we are checking whether we returned 2 which means that point is inside the polygon
        }
    }
    if(d1 == 0){
        if(pi.first >= point.first && 
            std::min(pi.second, pi1.second) <= point.second && 
            point.second <= std::max(pi.second, pi1.second)) {
            int d5 = det(point, q, pi2);
            int d6 = det(point, q, pi_1);
            if ((d5 > 0 && d6 < 0) || (d5 < 0 && d6 > 0)) {
                return 1;
            } else {
                return 0;
            }
        }
    }   
    return 0;
}



bool Country::rayCasting(std::vector<std::pair<int,int>> pointVec,std::pair<int,int> point){

    //ORDER OF POINTS IN POINTVEC MATTERS!!!//

    ////////////////Creating ray pq where q=(maxX+1,y), by finding max value of x int polygon/////////////////////////////////////////////
    ////////////////Simultaneously we are checking whether we can easily see that point is out of maximal bounds of polygon///////////////
    ////////////////If point is inside the maximal bounds of polygon that does not mean its inside the polygon////////////////////////////
    std::pair<int,int> tempPair1 = *std::min_element( pointVec.begin(),pointVec.end(), CompareByX());
    std::pair<int,int> tempPair2 = *std::max_element( pointVec.begin(),pointVec.end(), CompareByX());

    std::pair<int,int> tempPair3 = *std::min_element( pointVec.begin(),pointVec.end(), CompareByY());
    std::pair<int,int> tempPair4 = *std::max_element( pointVec.begin(),pointVec.end(), CompareByY());
    
    int minX = tempPair1.first;
    int maxX = tempPair2.first;
    int minY = tempPair3.second;
    int maxY= tempPair4.second;
    
    if(point.first > maxX || point.first < minX ){//point is not in the polygon
        return false;
    }

    if(point.second > maxY || point.second < minY ){//point is not in the polygon
        return false;
    }
    
    std::pair<int,int> q;//create "ray" that is represented by a line segment as maxX+1 is a value of x outside of the polygon
    q.first = maxX+1;
    q.second = point.second;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////iterate through each side//////////////////////////////////////////////////////////////////////////
    int crossCounter = 0;
    int vecLen = pointVec.size();
    
    for(int i=0;i<vecLen;++i){//for each side of the polygon, check if q crosses and add to crossCounter how many times it crosses
       std::pair<int,int> pi = pointVec[i];
        std::pair<int,int> pi1 = pointVec[(i+1)%vecLen];
        std::pair<int,int> pi2 = pointVec[(i+2)%vecLen];
        std::pair<int,int> pi_1 = pointVec[(i-1+vecLen)%vecLen];
        int result = crosses(point, q, pi, pi1, pi2, pi_1);

        if(result==2){
            return true;
        }
        crossCounter+=result;
        
    }
    //////////////////////odd number of crosses means that its inside otherwise its outside///////////////////////////////////////////////////
    if(crossCounter%2==1){
        return true;
    }
    else{
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

std::pair<int,std::vector<std::shared_ptr<Node>>> Country::mcmf(std::vector<std::shared_ptr<Node>> fromVec, std::vector<std::shared_ptr<Node>> &toVec, int convRate){
    std::map<std::shared_ptr<Node>, std::vector<Lane>> adjListCopy;
    
    // Copy original edges with reverse edges
    for (auto& [node, lanes] : adjList) {
        for (auto& lane : lanes) {
            adjListCopy[lane.getFromPtr()].push_back(lane);
            
            // Add reverse edge
            adjListCopy[lane.getToPtr()].push_back(
                Lane(lane.getToPtr(), lane.getFromPtr(), 0, -lane.getRepairCost()));
        }
    }

    auto superSource = std::make_shared<Node>();
    auto superSink = std::make_shared<Node>();

    // Connect super source to sources
    for (const auto& source : fromVec) {
        int capacity = 0;
        if (auto field = std::dynamic_pointer_cast<Field>(source)) {
            capacity = field->getProduction();
        } else if (auto brewery = std::dynamic_pointer_cast<Brewery>(source)) {
            capacity = brewery->getBeerAmount();
        }
        adjListCopy[superSource].emplace_back(superSource, source, capacity, 0);
        adjListCopy[source].emplace_back(source, superSource, 0, 0); // Reverse edge
    }

    // Connect sinks to super sink
    for (const auto& sink : toVec) {
        int capacity = 0;
        if (auto brewery = std::dynamic_pointer_cast<Brewery>(sink)) {
            capacity = brewery->getBarleyCap();
        } else if (auto pub = std::dynamic_pointer_cast<Pub>(sink)) {
            capacity = pub->getCapacity();
        }
        adjListCopy[sink].emplace_back(sink, superSink, capacity, 0);
        adjListCopy[superSink].emplace_back(superSink, sink, 0, 0); // Reverse edge
    }

    int total_cost = 0;
    int total_flow = 0;
    std::vector<std::shared_ptr<Node>> best_path;
    int best_path_cost = INT_MAX;
    int best_path_flow = 0;

    // Potential function for reduced costs
    std::unordered_map<std::shared_ptr<Node>, int> potential;
    for (auto& node : adjListCopy) {
        potential[node.first] = 0;
    }

    while (true) {
        //Dijkstra with potentials
        std::unordered_map<std::shared_ptr<Node>, int> dist;
        std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Node>> parent;
        std::unordered_map<std::shared_ptr<Node>, Lane*> parent_lane;
        
        //Init distances to max and source dist to 0
        for(auto& node : adjListCopy){
            dist[node.first] = INT_MAX;
        }
        dist[superSource] = 0;
        //Priority queue with el(distance, node) and greater that makes nodes with the shortest dist(cost) be put on top 
        //Note that we store int only because of the fact that its priority_queue that looks for the shortest dist(cost) 
     std::priority_queue<std::pair<int, std::shared_ptr<Node>>,std::vector<std::pair<int, std::shared_ptr<Node>>>,std::greater<std::pair<int, std::shared_ptr<Node>>>> pq;

        pq.push({0, superSource});

        while(!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;

            for (auto& lane : adjListCopy[u]) {
                if (lane.getFlow() >= lane.getCapacity()) continue;
                
                auto v = lane.getToPtr();
                int reduced_cost = lane.getRepairCost() + potential[u] - potential[v];
                int new_dist = dist[u] + reduced_cost;
                
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    parent[v] = u;
                    parent_lane[v] = &lane;
                    pq.push({new_dist, v});
                }
            }
        }

        if(dist[superSink] == INT_MAX) break;

        // Update potentials
        for (auto& node : adjListCopy) {
            if (dist[node.first] < INT_MAX) {
                potential[node.first] += dist[node.first];
            }
        }

        //Find minimum residual capacity and reconstruct path
        int path_flow = INT_MAX;
        std::vector<std::shared_ptr<Node>> currentPath;
        std::vector<Lane*> pathLanes;
        int current_path_cost = 0;
        
        for(auto v = superSink; v != superSource; v = parent[v]){
            pathLanes.push_back(parent_lane[v]);
            currentPath.push_back(v);
            if(path_flow>parent_lane[v]->getCapacity() - parent_lane[v]->getFlow()){
                path_flow = parent_lane[v]->getCapacity() - parent_lane[v]->getFlow();
            }
        }
        currentPath.push_back(superSource);
        std::reverse(currentPath.begin(), currentPath.end());

        //Path cost
        for (auto* lane : pathLanes) {
            current_path_cost += lane->getRepairCost();
        }
        //find  best path
        if(path_flow > best_path_flow || (path_flow == best_path_flow && current_path_cost < best_path_cost)) {
            best_path_flow = path_flow;
            best_path_cost = current_path_cost;
            best_path = currentPath;
        }

        //Augment flow
        //cost is calculated by multiplying with flow
        for (auto* lane : pathLanes) {
            lane->setFlow(lane->getFlow() + path_flow);
            total_cost += path_flow * lane->getRepairCost();
            
            //Update reverse edge
            for (auto& rev_lane : adjListCopy[lane->getToPtr()]) {
                if (rev_lane.getToPtr() == lane->getFromPtr()) {
                    rev_lane.setFlow(rev_lane.getFlow() - path_flow);
                    break;
                }
            }
        }

        total_flow += path_flow;
    }

    //Update sink nodes
    for(auto& sink : toVec) {
        for(auto& lane : adjListCopy[sink]){
            if(lane.getToPtr() == superSink){
                if(auto brewery = std::dynamic_pointer_cast<Brewery>(sink)){
                    brewery->setBarley(lane.getFlow());
                    brewery->conversion(convRate);
                }else if (auto pub = std::dynamic_pointer_cast<Pub>(sink)) {
                    pub->setCapacity(lane.getFlow());
                }
                break;
            }
        }
    }

    return {total_cost,best_path};
}
