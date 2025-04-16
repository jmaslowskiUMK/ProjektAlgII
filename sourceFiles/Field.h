#pragma once
#include "Node.h"

class Field:public Node
{
private:
	int production;
	int xMiddle, yMiddle,radius;
public:
	Field();
	Field(int x);
	Field(std::string ID, int production, int xMiddle, int yMiddle, int radius);
	int getProduction();
	void setProduction(int x);
	void print()override;
};

