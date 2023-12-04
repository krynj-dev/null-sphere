#include <vector>
#include <string>

using namespace std;

namespace aoc23
{
    std::vector<std::string> read_file(const std::string& filename);

    std::vector<std::string> split_string(std::string s, std::string delimiter);

    void run_day(int day);
} 
