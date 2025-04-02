#pragma once
#include <vector>
#include "Node.h"

class Brewery: public Node
{
private:
	int xMiddle, yMiddle,radius;
	int barleyAmount;
	int beerAmount;
public:
	Brewery();
	Brewery(int xMiddle,int yMiddle,int radius,int barleyAmount);
	int getBeerAmount();
	void setBarley(int barley);
	void print();
	void setBeer(int x);
};

