#include "Pub.h"
#include <iostream>

Pub::Pub(int ID, int xMiddle,int yMiddle, int radius, int capacity) {
	this->ID = ID;
	this->xMiddle = xMiddle;
	this->yMiddle = yMiddle;
	this->radius = radius;
	this->capacity = capacity;
}

void Pub::print() {
	std::cout << "x: " << xMiddle << std::endl;
	std::cout << "y: " << yMiddle << std::endl;
	std::cout << "radius: " << radius << std::endl;
	std::cout << "adjacent: ";

	std::cout << std::endl;
}

Pub::Pub() {}

int Pub::getCapacity() {
	return capacity;
}

void Pub::setCapacity(int capacity) {
	this->capacity = capacity;
}
