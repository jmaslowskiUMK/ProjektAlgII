#pragma once
#include <vector>
#include "Node.h"

class Pub: public Node
{
private:
	int xMiddle, yMiddle,radius;
public:
	Pub(std::string ID, int xMiddle,int yMiddle,int radius);
	void print()override;
};

