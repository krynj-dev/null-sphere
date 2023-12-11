#include "advent.h"
#include "util.h"

using namespace std;
using namespace aoc23;

void print_day(size_t, pair<long long, long long>);

int main(int argc, char* argv[]) {
    cout << "ADVENT OF CODE 2023" << endl;
    setvbuf (stdout, NULL, _IONBF, 0);

    print_day(1, day_1());
    print_day(2, day_2());
    print_day(3, day_3());
    print_day(4, day_4());
    print_day(5, day_5());
    print_day(6, day_6());
    print_day(7, day_7());
    print_day(8, day_8());
    print_day(9, day_9());
    print_day(10, day_10());
    print_day(11, day_11());

    return 0;
}

void print_day(size_t i, pair<long long, long long> answer)
{
    printf("Day %i:\tTask 1 = %15ld, Task 2 = %15ld\n", i, answer.first, answer.second);
}
