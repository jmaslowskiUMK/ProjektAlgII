#include "Brewery.h"
#include <iostream>

Brewery::Brewery() {
	beerAmount = 0;
}

Brewery::Brewery(std::string ID, int xMiddle, int yMiddle, int radius, int barleyAmount) {
	this->ID = ID;
	this->xMiddle = xMiddle;
	this->yMiddle = yMiddle;
	this->radius = radius;
	this->barleyAmount = barleyAmount;
}

int Brewery::getBeerAmount() {
	return beerAmount;
}

void Brewery::setBarley(int barley) {
	barleyAmount = barley;
}


void Brewery::print() {
	std::cout << "x: " << xMiddle << std::endl;
	std::cout << "y: " << yMiddle << std::endl;
	std::cout << "radius: " << radius << std::endl;
	std::cout << "barley ammount"<< barleyAmount<<std::endl;
	std::cout << "adjacent: ";
	std::cout << std::endl;
}

void Brewery::setBeer(int x) {
	beerAmount = x;
}
