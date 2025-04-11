#pragma once
#include <string>

class Node {
    std::string name;
public:
    virtual ~Node() = default;
    virtual void print();
    void printName();
    void setName(std::string name);
};
