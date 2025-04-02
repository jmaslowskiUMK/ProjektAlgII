#pragma once
#include <vector>
#include "Node.h"

class Intersection: public Node
{
private:
	int x;
	int y;
	int flow;
public:
	Intersection(int x, int y, int flow);
	Intersection(int x, int y);
	void setFlow(int flow);
	void print();
};

