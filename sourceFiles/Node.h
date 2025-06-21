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
    void setName(std::string name);
    void setX(int xMiddle);
    void setY(int yMiddle);
    std::string getName();
    int getID();
    int getX();
    int getY();
};
