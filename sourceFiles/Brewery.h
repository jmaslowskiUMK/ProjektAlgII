#pragma once
#include <vector>
#include "Node.h"

class Brewery: public Node
{
private:
	int barleyAmount;
	int beerAmount;
public:
	Brewery();
	Brewery(int ID, int xMiddle,int yMiddle,int radius,int barleyAmount);
	int getBeerAmount();
	void setBarley(int barley);
	void print()override;
	void setBeer(int x);
	void conversion();
};

