#pragma once
#include <vector>
#include "Node.h"

class Pub: public Node
{
private:
public:
	Pub();
	Pub(int ID, int xMiddle,int yMiddle,int radius);
	void print()override;
};

