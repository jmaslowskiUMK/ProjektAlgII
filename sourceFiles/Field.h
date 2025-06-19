#pragma once
#include "Node.h"

class Field:public Node
{
public:
	int production;
	Field();
	Field(int x);
	Field(int ID, int production, int xMiddle, int yMiddle, int radius);
	int getProduction();
	void setProduction(int x);
	void print()override;
};

