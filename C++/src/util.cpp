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
        day_2();    
        break;
    case 3:
        day_3();    
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
            lines.push_back(line.substr(0, line.size()));
        }

        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return lines;
}

std::vector<std::string> aoc23::split_string(std::string s, std::string delimiter) {
    std::vector<std::string> words;
    std::string s_cpy = s;
    size_t pos = 0;
    string token;
    while ((pos = s_cpy.find(delimiter)) != std::string::npos) {
        token = s_cpy.substr(0, pos);
        s_cpy.erase(0, pos + delimiter.length());
        words.push_back(token);
    }
    words.push_back(s_cpy);
    return words;
}