#pragma once
#include <vector>
#include <utility>

class Hull{
public:
	std::vector<std::pair<int,int>> points;
	Hull();
	void addPoint(std::pair<int,int> auxPair);
	void print();
};
