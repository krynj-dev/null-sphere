#include "advent.h"
#include "util.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;
using namespace aoc23;

int main(int argc, char* argv[]) {
    cout << "ADVENT OF CODE 2023" << endl;

    for (int i = 1; i < 6; i++) {
        run_day(i);
    }

    return 0;
}