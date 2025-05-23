#pragma once
#include <vector>
#include "Node.h"

class Intersection: public Node
{
private:
	int x,y,flow;
public:
	Intersection(int x, int y, int flow);
	Intersection(int x, int y);
	void print()override;
};

