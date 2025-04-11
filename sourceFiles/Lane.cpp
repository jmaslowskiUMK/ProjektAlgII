#include "Lane.h"

Lane::Lane() {
	from = nullptr;
	to = nullptr;
	flow = 0;
}
Lane::~Lane() {

}

Lane::Lane(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int flow) {
	this->from = from;
	this->to = to;
	this->flow = flow;
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