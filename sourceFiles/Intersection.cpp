#include "Intersection.h"
#include <iostream>

Intersection::Intersection(int x, int y ) {
	this->x = x;
	this->y = y;
}

void Intersection::print() {
	std::cout << "x: " << x << std::endl;
	std::cout << "y: " << y << std::endl;
	std::cout << "adjacent: ";

	std::cout << std::endl;
}
