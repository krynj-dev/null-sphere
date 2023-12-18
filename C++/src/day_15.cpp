#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

namespace day_15
{
    long long task_1(vector<string>);
    long long task_2(vector<string>);
    vector<string> parse_values(vector<string>);
}

size_t do_hash(string, long long, long long);
long long focusing_power(vector<vector<pair<string, long long>>>);

pair<long long, long long> aoc23::day_15()
{
    vector<string> lines = read_file("resources/input_15.txt", true);
    vector<string> init_seq = day_15::parse_values(lines);

    long long task_1_answer = day_15::task_1(init_seq);

    long long task_2_answer = day_15::task_2(init_seq);

    return {task_1_answer, task_2_answer};
}

long long day_15::task_1(vector<string> init_seq)
{
    // Put stuff here
    int x = 0;

    for (string seq_part : init_seq)
    {
        x += do_hash(seq_part, 17, 256);
    }

    return x;
}

long long day_15::task_2(vector<string> lines)
{
    vector<vector<pair<string, long long>>> hashm(256);
    long long x = 0;

    for (string str: lines)
    {
        size_t splitting_idx = str.find_first_of("-=");
        string key = str.substr(0, splitting_idx);
        char action = str[splitting_idx];
        size_t idx = do_hash(key, 17, 256);
        if (action == '=')
        {
            auto it = find_if(hashm[idx].begin(), hashm[idx].end(), [key](pair<string, long long> a)
            {
                return a.first == key;
            });
            if (it != hashm[idx].end())
            {
                (*it) = {key, stoll(str.substr(splitting_idx+1))};
            }
            else
            {
                hashm[idx].push_back({key, stoll(str.substr(splitting_idx+1))});
            }
        }
        else if (action == '-')
        {
            auto it = find_if(hashm[idx].begin(), hashm[idx].end(), [key](pair<string, long long> a)
            {
                return a.first == key;
            });
            if (it != hashm[idx].end())
            {
                hashm[idx].erase(it);
            }
        }
    }

    return focusing_power(hashm);
}

size_t do_hash(string str, long long a, long long b)
{
    long long x = 0;
    for (char c : str)
    {
        x = (x + (int)c)*a % b;
    }
    return x;
}

vector<string> day_15::parse_values(vector<string> lines)
{
    return split_string(lines[0], ",");
}

long long focusing_power(vector<vector<pair<string, long long>>> boxes)
{
    long long fp = 0;
    for (size_t i = 0; i < boxes.size(); ++i)
    {
        for (size_t j = 0; j < boxes[i].size(); ++j)
        {
            fp += (i+1)*(j+1)*boxes[i][j].second;
        }
    }
    return fp;
}
