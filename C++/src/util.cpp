#include "util.h"
#include "advent.h"
#include <iostream>
#include <fstream>

using namespace aoc23;
using namespace std;

void aoc23::run_day(int day) {
    std::cout << "Running day " << day << std::endl;
    switch (day)
    {
    case 1:
        day_1();
        break;
    case 2:
        break;
    default:
        break;
    }
}

std::vector<std::string> aoc23::read_file(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return lines;
}