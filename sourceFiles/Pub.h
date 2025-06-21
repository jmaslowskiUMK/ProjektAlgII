#pragma once
#include <vector>
#include "Node.h"

class Pub: public Node
{
private:
public:
	int capacity;
	Pub();
	Pub(int ID, int xMiddle,int yMiddle,int radius, int capacity);
	void print()override;
	int getCapacity();
	void setCapacity(int capacity);
};

