#include "Intersection.h"
#include <iostream>

Intersection::Intersection(int x, int y,int flow ) {
	this->x = x;
	this->y = y;
	this->flow = flow;
}

Intersection::Intersection(int x, int y {
	this->x = x;
	this->y = y;
	this->flow = 0;
}

void Intersection::setFlow(int flow) {
	this->flow = flow;
}


void Intersection::print() {
	std::cout << "x: " << x << std::endl;
	std::cout << "y: " << y << std::endl;
	std::cout << "flow " << flow << std::endl;
	std::cout << "adjacent: ";

	std::cout << std::endl;
}