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
public:
	Lane();
	~Lane();
	Lane(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int flow);
	std::shared_ptr<Node> getFromPtr();
	std::shared_ptr<Node> getToPtr();
	int getFlow();
	void setFlow(int flow);
};

