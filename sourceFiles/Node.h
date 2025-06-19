#pragma once
#include <string>

class Node {
public:
    int production;
    int xMiddle, yMiddle, radius;
    int ID;
    std::string name;

    virtual ~Node() = default;
    virtual void print();
    void printName();
    int getID();
    std::string getName();
    void setName(std::string name);
    int getX();
    int getY();
};
