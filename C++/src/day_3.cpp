#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

namespace day_3
{
    long long task_1(map<pair<size_t, size_t>, vector<int>>);
    long long task_2(map<pair<size_t, size_t>, vector<int>>, vector<string>);

} // namespace day_3

map<pair<size_t, size_t>, vector<int>> get_part_numbers(vector<string>, bool);
pair<int, int> get_number_from_digit(string, size_t, size_t, vector<pair<size_t, size_t>> &);

pair<long long, long long> aoc23::day_3()
{
    std::vector<std::string> lines = read_file("resources/input_3.txt", true);

    // Task 1
    map<pair<size_t, size_t>, vector<int>> parts_1 = get_part_numbers(lines, true);
    int total = day_3::task_1(parts_1);

    // Task 2
    map<pair<size_t, size_t>, vector<int>> parts_2 = get_part_numbers(lines, false);
    int total2 = day_3::task_2(parts_2, lines);

    return {total, total2};
}

long long day_3::task_1(map<pair<size_t, size_t>, vector<int>> parts)
{
    return accumulate(parts.begin(), parts.end(), 0LL, [](long long acc, pair<pair<size_t, size_t>, vector<int>> kv)
    {
        return acc + accumulate(kv.second.begin(), kv.second.end(), 0);
    });
}

long long day_3::task_2(map<pair<size_t, size_t>, vector<int>> parts, vector<string> lines)
{
    return accumulate(parts.begin(), parts.end(), 0LL, [lines](long long acc, pair<pair<size_t, size_t>, vector<int>> kv)
    {
        return acc + ((lines[kv.first.second][kv.first.first] == '*' && kv.second.size() == 2) ? kv.second[0]*kv.second[1] : 0);
    });
}

map<pair<size_t, size_t>, vector<int>> get_part_numbers(vector<string> lines, bool ignore_duplicates)
{
    map<pair<size_t, size_t>, vector<int>> parts;
    vector<pair<size_t, size_t>> visited;
    for (int y = 0; y < lines.size(); ++y)
    {
        string line = lines[y];
        for (int i = line.find_first_not_of("0123456789.", 0); i != string::npos; i = line.find_first_not_of("0123456789.", i + 1))
        {
            for (int z = max(0, y - 1); z <= min((int)lines.size() - 1, y + 1); ++z)
            {
                int x = max(0, i - 1);
                while (x <= min((int)line.size() - 1, i + 1))
                {
                    pair<size_t, size_t> coord = {x, z};
                    if ((find(visited.begin(), visited.end(), coord) == visited.end() || !ignore_duplicates) && string(1, lines[z][x]).find_first_of("0123456789") != string::npos)
                    {
                        pair<int, int> to_add = get_number_from_digit(lines[z], x, z, visited);
                        parts[{i, y}].push_back(stoi(lines[z].substr(to_add.first, to_add.second - to_add.first + 1)));
                        x = to_add.second + 1;
                    }
                    else
                    {
                        ++x;
                    }
                }
            }
        }
    }

    return parts;
}

pair<int, int> get_number_from_digit(string line, size_t x, size_t y, vector<pair<size_t, size_t>> &visited)
{
    // scan left
    int num_start = x;
    int num_end = x;
    visited.push_back({x, y});
    while (num_start - 1 >= 0 && string(1, line[num_start - 1]).find_first_of("0123456789") != string::npos)
    {
        num_start--;
        visited.push_back({num_start, y});
    }
    while (num_end + 1 < line.size() && string(1, line[num_end + 1]).find_first_of("0123456789") != string::npos)
    {
        num_end++;
        visited.push_back({num_end, y});
    }

    return {num_start, num_end};
}
