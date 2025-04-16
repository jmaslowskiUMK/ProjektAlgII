#include "Node.h"
#include <iostream>

void Node::print() {
    std::cout << "Node " << std::endl;
}

void Node::printName() {
    std::cout << ID << std::endl;
}

void Node::setName(std::string name) {
    this->ID = name;
}

std::string Node::getID() {
    return this->ID;
}