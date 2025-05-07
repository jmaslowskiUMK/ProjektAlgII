#include "Pub.h"
#include <iostream>

Pub::Pub(int ID, int xMiddle,int yMiddle, int radius) {
	this->ID = ID;
	this->xMiddle = xMiddle;
	this->yMiddle = yMiddle;
	this->radius = radius;
}

void Pub::print() {
	std::cout << "x: " << xMiddle << std::endl;
	std::cout << "y: " << yMiddle << std::endl;
	std::cout << "radius: " << radius << std::endl;
	std::cout << "adjacent: ";

	std::cout << std::endl;
}

Pub::Pub() {}