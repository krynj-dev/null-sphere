#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

namespace day_1
{
    long long task_1(vector<string>);
    long long task_2(vector<string>);
}

int num_from_line(string line, bool);

pair<long long, long long> aoc23::day_1()
{
    vector<string> lines = read_file("resources/input_1.txt", true);

    long long task_1_answer = day_1::task_1(lines);

    long long task_2_answer = day_1::task_2(lines);

    return {task_1_answer, task_2_answer};
}

long long day_1::task_1(vector<string> lines)
{
    return accumulate(lines.begin(), lines.end(), 0, [](int acc, string line)
                      { return acc + num_from_line(line, false); });
}

long long day_1::task_2(vector<string> lines)
{
    return accumulate(lines.begin(), lines.end(), 0, [](int acc, string line)
                      { return acc + num_from_line(line, true); });
}

int num_from_line(string line, bool include_number_words)
{
    string number_string = "";
    map<string, char> number_words =
        {{"one", '1'}, {"two", '2'}, {"three", '3'}, {"four", '4'}, {"five", '5'}, {"six", '6'}, {"seven", '7'}, {"eight", '8'}, {"nine", '9'}};
    size_t first_digit_loc = line.find_first_of("0123456789");
    char digit_tens = line[first_digit_loc];
    if (include_number_words)
    {
        pair<char, size_t> num = {digit_tens, first_digit_loc};
        for (auto it = number_words.begin(); it != number_words.end(); ++it)
        {
            auto pos = line.find(it->first);
            if (pos < num.second)
            {
                num = {it->second, pos};
            }
        }
        digit_tens = num.first;
        first_digit_loc = num.second;
    }
    size_t second_digit_loc = line.find_last_of("0123456789");
    char digit_ones = line[second_digit_loc];
    if (include_number_words)
    {
        pair<char, size_t> num = {digit_ones, second_digit_loc};
        for (auto it = number_words.begin(); it != number_words.end(); ++it)
        {
            auto pos = line.rfind(it->first);
            if (pos > num.second && pos != string::npos)
            {
                num = {it->second, pos};
            }
        }
        digit_ones = num.first;
        second_digit_loc = num.second;
    }
    number_string += digit_tens;
    number_string += digit_ones;
    return stoi(number_string);
}
