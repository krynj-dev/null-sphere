#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

namespace day_13
{
    vector<vector<string>> parse_patterns(vector<string>);
    long long task_1(vector<vector<string>>);
    long long task_2(vector<vector<string>>);
}

vector<size_t> reflection(vector<string>, int);

pair<long long, long long> aoc23::day_13()
{
    vector<string> lines = read_file("resources/input_13.txt", true);

    vector<vector<string>> patterns = day_13::parse_patterns(lines);

    long long task_1_answer = day_13::task_1(patterns);

    long long task_2_answer = day_13::task_2(patterns);

    return {task_1_answer, task_2_answer};
}

long long day_13::task_1(vector<vector<string>> patterns)
{
    long long x = 0;
    vector<vector<size_t>> diff_count;
    for (auto pat : patterns)
    {
        vector<size_t> a = reflection(pat, 0);
        vector<size_t> b = reflection(pat, 1);
        for (size_t i = 0; i < a.size(); ++i)
        {
            if (a[i] == 0)
            {
                x += i + 1;
            }
        }
        for (size_t j = 0; j < b.size(); ++j)
        {
            if (b[j] == 0)
            {
                x += 100 * (j + 1);
            }
        }
    }
    return x;
}

long long day_13::task_2(vector<vector<string>> patterns)
{
    long long x = 0;
    vector<vector<size_t>> diff_count;
    for (auto pat : patterns)
    {
        vector<size_t> a = reflection(pat, 0);
        vector<size_t> b = reflection(pat, 1);
        for (size_t i = 0; i < a.size(); ++i)
        {
            if (a[i] == 1)
            {
                x += i + 1;
            }
        }
        for (size_t j = 0; j < b.size(); ++j)
        {
            if (b[j] == 1)
            {
                x += 100 * (j + 1);
            }
        }
    }
    return x;
}

vector<vector<string>> day_13::parse_patterns(vector<string> lines)
{
    vector<vector<string>> patterns;
    vector<string> pattern;
    for (vector<string>::iterator s = lines.begin(); s != lines.end(); ++s)
    {
        string s1 = *s;
        if (s1.empty() || next(s) == lines.end())
        {
            patterns.push_back(pattern);
            pattern = {};
        }
        else
        {
            pattern.push_back(s1);
        }
    }
    return patterns;
}

vector<size_t> reflection(vector<string> pattern, int axis) // axis=0 -> vert, axis=1 -> horiz
{
    size_t x = 0;
    size_t xi;
    size_t outer_bound;
    size_t inner_bound;
    switch (axis)
    {
    case 1:
        outer_bound = pattern.size();
        inner_bound = pattern[0].size();
        break;
    case 0:
    default:
        outer_bound = pattern[0].size();
        inner_bound = pattern.size();
    }
    vector<size_t> diff_list;
    for (; x < outer_bound - 1; ++x)
    {
        size_t diff_count = 0;
        for (size_t y = 0; y < inner_bound; ++y)
        {
            for (xi = 1; xi - 1 <= x && x + xi < outer_bound; ++xi) // x=3,2,1,0 | xi=1(4),2(5),3(6),4(7)
            {
                char char1;
                char char2;
                if (axis == 0)
                {
                    char1 = pattern[y][x + 1 - xi];
                    char2 = pattern[y][x + xi];
                }
                else if (axis == 1)
                {
                    char1 = pattern[x + 1 - xi][y];
                    char2 = pattern[x + xi][y];
                }
                if (char1 != char2)
                {
                    ++diff_count;
                }
            }
        }
        diff_list.push_back(diff_count);
    }
    return diff_list;
}
