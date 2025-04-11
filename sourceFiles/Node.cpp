#include "Node.h"
#include <iostream>

void Node::print() {
    std::cout << "Node " << std::endl;
}

void Node::printName() {
    std::cout << name << std::endl;
}

void Node::setName(std::string name) {
    this->name = name;
}
