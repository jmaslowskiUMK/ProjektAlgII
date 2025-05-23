#include "Lane.h"

Lane::Lane() {
	from = nullptr;
	to = nullptr;
	flow = 0;
}
Lane::~Lane() {

}

Lane::Lane(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int capacity, int repair_cost) {
	this->from = from;
	this->to = to;
	this->capacity = capacity;
	this->repair_cost = repair_cost;
	flow=0;
}

std::shared_ptr<Node> Lane::getFromPtr() {
	return from;
}

std::shared_ptr<Node> Lane::getToPtr() {
	return to;
}

int Lane::getFlow() {
	return flow;
}

void Lane::setFlow(int flow) {
	this->flow = flow;
}

int Lane::getRepairCost() {
	return repair_cost;
}

int Lane::getCapacity() {
	return capacity;
}

void Lane::setCapacity(int capacity) {
	this->capacity = capacity;
}
