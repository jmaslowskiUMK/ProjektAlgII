#pragma once
#include <vector>
#include <utility>

class Hull{
private:
	std::vector<std::pair<int,int>> points;
public:
	void addPoint(std::pair<int,int> auxPair);
};
