#include <advent.h>
#include <util.h>
#include <iostream>
#include <string>
#include <regex>

std::regex get_task_1_regex();
std::regex get_task_2_regex();
std::string digits_from_line(std::string line, bool include_written_numbers);
int get_total(std::vector<string> lines, bool include_written_numbers);
int number_from_first_last_digits(std::string digit_line);

void aoc23::day_1()
{
    std::vector<std::string> lines = aoc23::read_file("resources/input_1.txt", false);

    int total_task_1 = get_total(lines, false);

    cout << "\tTask 1: total is " << total_task_1 << endl;

    int total_task_2 = get_total(lines, true);

    cout << "\tTask 2: total is " << total_task_2 << endl;
}

std::string digits_from_line(std::string line, bool include_written_numbers)
{
    std::string line_cpy = line;
    std::regex regex;

    if (include_written_numbers)
    {
        regex = std::regex("(?=([0-9]|one|two|three|four|five|six|seven|eight|nine)).");
    }
    else
    {
        regex = std::regex("[0-9]");
    }

    std::string number_string = "";

    auto digits_start = std::sregex_iterator(line_cpy.begin(), line_cpy.end(), regex);
    auto digits_end = std::sregex_iterator();

    for (std::sregex_iterator i = digits_start; i != digits_end; ++i)
    {
        std::smatch match = *i;
        std::string match_str;
        if (include_written_numbers)
        {
            match_str = match.str(1);
        }
        else
        {
            match_str = match.str();
        }
        number_string += match_str;
    }

    if (include_written_numbers)
    {
        number_string = std::regex_replace(number_string, std::regex("one"), "1");
        number_string = std::regex_replace(number_string, std::regex("two"), "2");
        number_string = std::regex_replace(number_string, std::regex("three"), "3");
        number_string = std::regex_replace(number_string, std::regex("four"), "4");
        number_string = std::regex_replace(number_string, std::regex("five"), "5");
        number_string = std::regex_replace(number_string, std::regex("six"), "6");
        number_string = std::regex_replace(number_string, std::regex("seven"), "7");
        number_string = std::regex_replace(number_string, std::regex("eight"), "8");
        number_string = std::regex_replace(number_string, std::regex("nine"), "9");
    }

    return number_string;
}

int number_from_first_last_digits(std::string digit_line)
{
    if (digit_line.size() > 0)
    {
        std::string digit_first(1, digit_line[0]);
        std::string digit_last(1, digit_line[digit_line.size() - 1]);
        return stoi(digit_first + digit_last);
    }
    else
    {
        return 0;
    }
}

int get_total(std::vector<string> lines, bool include_written_numbers)
{
    std::vector<int> calibration_values = {};

    for (int i = 0; i < lines.size(); i++)
    {
        std::string digits = digits_from_line(lines[i], include_written_numbers);
        if (digits.size() > 0)
        {
            int cal_val = number_from_first_last_digits(digits);
            calibration_values.push_back(cal_val);
        }
    }

    int total = 0;
    for (const int &value : calibration_values)
    {
        total += value;
    }

    return total;
}
