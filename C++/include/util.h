#include <vector>
#include <string>

namespace aoc23
{
    std::vector<std::string> read_file(const std::string& filename, const bool strip_newline);

    std::vector<std::string> split_string(std::string s, std::string delimiter);

    std::vector<long long> parse_numbers(std::string line);
} 
