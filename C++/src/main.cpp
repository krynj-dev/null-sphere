#include "advent.h"
#include "util.h"

using namespace std;
using namespace aoc23;

int main(int argc, char* argv[]) {
    cout << "ADVENT OF CODE 2023" << endl;
    setvbuf (stdout, NULL, _IONBF, 0);

    for (int i = 1; i < 10; i++) {
        run_day(i);
    }

    return 0;
}