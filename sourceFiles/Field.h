#pragma once
#include "Node.h"

class Field:public Node
{
private:
	int production;
public:
	Field();
	Field(int x);
	Field(int ID, int production, int xMiddle, int yMiddle, int radius);
	int getProduction();
	void setProduction(int x);
	void print()override;
};

