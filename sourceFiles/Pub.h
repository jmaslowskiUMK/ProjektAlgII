#pragma once
#include <vector>
#include "Node.h"

class Pub: public Node
{
private:
public:
	Pub(std::string ID, int xMiddle,int yMiddle,int radius);
	void print()override;
};

