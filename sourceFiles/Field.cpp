#include "Field.h"
#include <iostream>

Field::Field() {
	production = 0;
}

Field::Field(int x) {
	production = x;
}

int Field::getProduction() {
	return production;
}

int Field::getBasicProduction() {
	return basicProduction;
}

void Field::setProduction(int x){
	production = x;
}

Field::Field(int ID, int production, int xMiddle, int yMiddle, int radius) {
	this->ID = ID;
	this->xMiddle = xMiddle;
	this->yMiddle = yMiddle;
	this->radius = radius;
	this->production = production;
	this->basicProduction = production;
}

void Field::print() {
	std::cout << "x: " << xMiddle << std::endl;
	std::cout << "y: " << yMiddle << std::endl;
	std::cout << "radius: " << radius << std::endl;
	std::cout << "production " << production << std::endl;
	std::cout << "adjacent: ";

	std::cout << std::endl;
}
