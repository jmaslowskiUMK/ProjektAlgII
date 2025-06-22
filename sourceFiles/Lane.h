#pragma once
#include <memory>
#include "Node.h"
#include <map>
#include <vector>

class Lane
{
private:
	std::shared_ptr<Node> from;
	std::shared_ptr<Node> to;
	int flow;
	int capacity;
	int repair_cost=0;
public:
	Lane();
	~Lane();
	Lane(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int capacity, int repair_cost);
	std::shared_ptr<Node> getFromPtr();
	std::shared_ptr<Node> getToPtr();
	int getRepairCost();
	int getFlow();
	void setFlow(int flow);
	int getCapacity(); 
	void setCapacity(int capacity);
};

