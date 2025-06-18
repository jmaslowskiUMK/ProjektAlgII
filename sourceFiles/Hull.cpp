#include "Hull.h"
#include <iostream>

Hull::Hull(int groundClass) {
	this->groundClass = groundClass;
}

Hull::~Hull() {
	this->points.clear();
}

void Hull::addPoint(std::pair<int,int> auxPair){
	Hull::points.push_back(auxPair);
}

void Hull::print() {
	for (int i = 0; i < points.size(); i++) {
		std::cout << "x: " << points[i].first << " y: " << points[i].second << std::endl;
	}
}