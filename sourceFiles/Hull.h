#pragma once
#include <vector>
#include <utility>

class Hull{
public:
	int groundClass;

	std::vector<std::pair<int,int>> points;
	Hull(int groundClass);
	void addPoint(std::pair<int,int> auxPair);
	void print();
};
