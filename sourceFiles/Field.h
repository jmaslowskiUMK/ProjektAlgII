#pragma once
#include "Node.h"

class Field:public Node
{
public:
	int production;
	int basicProduction;
	int groundClass;
	Field();
	Field(int x);
	Field(int ID, int production, int xMiddle, int yMiddle, int radius);
	int getProduction();
	int getBasicProduction();
	void setProduction(int x);
	void print()override;
};

