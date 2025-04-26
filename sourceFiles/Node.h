#pragma once
#include <string>

class Node {
private:
public:
    int xMiddle, yMiddle, radius;

    std::string ID;
    virtual ~Node() = default;
    virtual void print();
    void printName();
    //nadanie ID (zmieniłem nazwę bo chodzi o element pozwalający na rozpoznanie) niech odbywa się w konstruktorze każdego obiektu dziedzicznego
    void setName(std::string name);
    std::string getID();
    int getX();
    int getY();
};
