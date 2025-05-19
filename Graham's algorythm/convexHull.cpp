#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm> //for remove_if 

struct Point {
    int x, y;
};

//random point for new standard of convex hull :)
Point randomStartPoint(const std::vector<Point>& points) {
    return points[rand() % points.size()];
}

bool isInsideCircle(const Point& center, const Point& p, int radius) {
    return std::sqrt(std::pow(p.x - center.x, 2) + std::pow(p.y - center.y, 2)) <= radius;
}

//determinant for orientation
// 0 - collinear, >0 - clockwise, <0 - counterclockwise
int det(const Point& p, const Point& q, const Point& r) {
    return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
}

std::vector<Point> convexHull(std::vector<Point>& points) {
    if (points.size() < 3) return {};

    std::stack<Point> hull;
    hull.push(points[0]);
    hull.push(points[1]);

    for (size_t i = 2; i < points.size(); ++i) {
        while (hull.size() >= 2) {
            Point second = hull.top(); hull.pop();
            Point first = hull.top();
            if (det(first, second, points[i]) < 0) {
                hull.push(second);
                break;
            }
        }
        hull.push(points[i]);
    }

    std::vector<Point> result;
    while (!hull.empty()) {
        result.push_back(hull.top());
        hull.pop();
    }
    return result;
}

//reader CSV
std::vector<Point> readFromCSV(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file");

    std::string line;
    std::getline(file, line); //skip header
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string x_str, y_str;
        if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',')) {
            points.push_back({std::stoi(x_str), std::stoi(y_str)});
        }
    }
    file.close();
    return points;
}

int main() {
    srand(time(0));
    std::string filename = "Points.csv";
    std::vector<Point> points = readFromCSV(filename);

    //get radius and hull count
    int radius, hullCount;
    std::cout << "Podaj promień okręgu: ";
    std::cin >> radius;
    std::cout << "Podaj liczbę otoczek do wygenerowania: ";
    std::cin >> hullCount;

    for (int i = 0; i < hullCount && !points.empty(); ++i) {
        Point center = randomStartPoint(points);

        std::vector<Point> filteredPoints;
        for (const auto& p : points) {
            if (isInsideCircle(center, p, radius)) {
                filteredPoints.push_back(p);
            }
        }

        if (filteredPoints.size() < 3) {
            std::cout << "Za mało punktów do utworzenia otoczki w okręgu " << i + 1 << "\n";
            continue;
        }

        auto hull = convexHull(filteredPoints);
        std::cout << "Otoczka wypukła dla okręgu " << i + 1 << " (środek: " << center.x << ", " << center.y << "):\n";
        for (const auto& p : hull) {
            std::cout << "(" << p.x << ", " << p.y << ")\n";
        }
        std::cout << std::endl;

        //remove if inside circle
        points.erase(std::remove_if(points.begin(), points.end(),
            [&](const Point& p) { return isInsideCircle(center, p, radius); }), points.end());
    }

    return 0;
}
