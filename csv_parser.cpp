#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

extern "C" {
    void processCSV(const char* csvData) {
        string data(csvData);
        stringstream ss(data);
        string line;

        while (getline(ss, line)) {
            stringstream lineStream(line);
            string cell;
            vector<string> row;

            while (getline(lineStream, cell, ',')) {
                row.push_back(cell);
            }

            for (const auto& cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }
}