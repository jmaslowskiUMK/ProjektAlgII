#include "Node.h"
#include <iostream>

void Node::print() {
    std::cout << "Node " << std::endl;
}

void Node::setX(int xMiddle) {
    this->xMiddle = xMiddle;
}

void Node::setY(int yMiddle) {
    this->yMiddle = yMiddle;
}

int Node::getID() {
    return this->ID;
}

int Node::getX() {
    return this->xMiddle;
}

int Node::getY() {
    return this->yMiddle;
}

void Node::printName() {
    if (this->ID % 3 == 0) {
        std::cout << "Field_" + std::to_string(ID / 3) << std::endl;
    }   else if (this->ID % 3 == 1) {
        std::cout << "Brewery_" + std::to_string(ID / 3) << std::endl;
    }   else if (this->ID % 3 == 2) {
        std::cout << "Pub_" + std::to_string(ID / 3) << std::endl;
    }   else {
        std::cout << "Name: " << this->name << std::endl;
    }
}

void Node::setName(std::string name) {
    this->name = name;
}

std::string Node::getName() {
    std::string str;

    if (this->ID % 3 == 0) {
        str = "Field_" + std::to_string(ID / 3);
    }   else if (this->ID % 3 == 1) {
        str = "Brewery_" + std::to_string(ID / 3);
    }   else if (this->ID % 3 == 2) {
        str = "Pub_" + std::to_string(ID / 3);
    }

    return str;
}