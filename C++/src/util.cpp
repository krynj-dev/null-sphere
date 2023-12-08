#include "util.h"
#include "advent.h"

using namespace aoc23;
using namespace std;

void aoc23::run_day(int day)
{
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
    case 4:
        day_4();
        break;
    case 5:
        day_5();
        break;
    case 6:
        day_6();
        break;
    case 7:
        day_7();
        break;
    case 8:
        day_8();
        break;
    default:
        break;
    }
}

std::vector<std::string> aoc23::read_file(const std::string &filename, const bool strip_newline)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            size_t endpoint = line.size();
            if (strip_newline)
            {
                endpoint = line.find_first_of("\r\n");
            }
            lines.push_back(line.substr(0, endpoint));
        }

        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return lines;
}

std::vector<std::string> aoc23::split_string(std::string s, std::string delimiter)
{
    std::vector<std::string> words;
    std::string s_cpy = s;
    size_t pos = 0;
    string token;
    while ((pos = s_cpy.find(delimiter)) != std::string::npos)
    {
        token = s_cpy.substr(0, pos);
        s_cpy.erase(0, pos + delimiter.length());
        words.push_back(token);
    }
    words.push_back(s_cpy);
    return words;
}

std::vector<long long> aoc23::parse_numbers(std::string line)
{
    vector<long long> numbers;
    std::regex nums(" *[0-9]+");
    for (std::sregex_iterator i = std::sregex_iterator(line.begin(), line.end(), nums);
         i != std::sregex_iterator();
         ++i)
    {
        std::smatch m = *i;
        string str = m.str();
        str.erase(std::remove_if(str.begin(), str.end(), [](char c)
                                 { return c == ' ' || c == '\t'; }),
                  str.end());
        numbers.push_back(stoll(str));
    }
    return numbers;
}
