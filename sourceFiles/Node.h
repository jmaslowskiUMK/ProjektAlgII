#pragma once
#include <string>

class Node {
private:
public:
    int xMiddle, yMiddle, radius;
    int ID;

    virtual ~Node() = default;
    virtual void print();
    void printName();
    int getID();
    std::string getName();
    int getX();
    int getY();
};
