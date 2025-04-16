#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "./sourceFiles/Country.h"
#include "./sourceFiles/Brewery.h"
#include "./sourceFiles/Pub.h"
#include "./sourceFiles/Field.h"
#include "./sourceFiles/Brewery.h"

#define ROW_LENGTH 11
#define CONST_RADIUS 10

using namespace std;


extern "C" {
    void processCSV(const char* csvData) {
        string data(csvData);
        stringstream ss(data);
        string line;

        Country objectKingdom;

        while (getline(ss, line)) {
            stringstream lineStream(line);
            string * row = new string[ROW_LENGTH];

            for (int i = 0; i < ROW_LENGTH; i++) {
                getline(lineStream, row[i], ',');
            }

            if (row[0] == "Field") {
                objectKingdom.createField(row[1], stoi(row[2]), stoi(row[5]), stoi(row[6]), CONST_RADIUS);
            }   else if (row[0] == "Brewery") {
                objectKingdom.createBrewery(row[1], stoi(row[3]), stoi(row[5]), stoi(row[6]), CONST_RADIUS);
            }   else if (row[0] == "Pub") {
                objectKingdom.createPub(row[1], stoi(row[5]), stoi(row[6]), CONST_RADIUS);
            }   else if (row[0] == "Lane") {
                auto from = objectKingdom.find(row[7]);
                auto to = objectKingdom.find(row[8]);

                if (from && to) {
                    Lane lane(from, to, stod(row[10]));
                    objectKingdom.addRelationship(lane);
                }
            }
        }
            objectKingdom.printContent();
            objectKingdom.printBfs(objectKingdom.find("Field_1"));
    }
}