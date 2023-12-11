#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

long long task_1(vector<size_t>, vector<size_t>, vector<pair<size_t, size_t>>);
long long task_2(vector<size_t>, vector<size_t>, vector<pair<size_t, size_t>>);

void parse_galaxies(vector<string>, vector<size_t> &, vector<size_t> &, vector<pair<size_t, size_t>> &);
vector<tuple<pair<size_t, size_t>, pair<size_t, size_t>, long long>> calculate_distances(vector<size_t>, vector<size_t>, vector<pair<size_t, size_t>>, size_t);

pair<long long, long long> aoc23::day_11()
{
    vector<string> lines = read_file("resources/input_11.txt", true);

    vector<size_t> expanded_columns;
    vector<size_t> expanded_rows;
    vector<pair<size_t, size_t>> galaxies;

    parse_galaxies(lines, expanded_columns, expanded_rows, galaxies);

    // for (auto num : expanded_columns)
    // {
    //     cout << num << ", ";
    // }
    // cout << endl;
    // for (auto num : expanded_rows)
    // {
    //     cout << num << ", ";
    // }
    // cout << endl;
    // for (auto gal : galaxies)
    // {
    //     cout << "(" << gal.first << ", " << gal.second << ")"
    //          << ", ";
    // }
    // cout << endl;

    long long task_1_answer = task_1(expanded_columns, expanded_rows, galaxies);

    long long task_2_answer = task_2(expanded_columns, expanded_rows, galaxies);

    return {task_1_answer, task_2_answer};
}

long long task_1(vector<size_t> expanded_columns, vector<size_t> expanded_rows, vector<pair<size_t, size_t>> galaxies)
{
    // Put stuff here

    long long x = 0;

    vector<tuple<pair<size_t, size_t>, pair<size_t, size_t>, long long>> ans = calculate_distances(expanded_columns, expanded_rows, galaxies, 1);

    for (auto t : ans)
    {
        x += get<2>(t);
    }

    return x;
}

long long task_2(vector<size_t> expanded_columns, vector<size_t> expanded_rows, vector<pair<size_t, size_t>> galaxies)
{
    // Put stuff here

    long long x = 0;

    vector<tuple<pair<size_t, size_t>, pair<size_t, size_t>, long long>> ans = calculate_distances(expanded_columns, expanded_rows, galaxies, 1000000);

    for (auto t : ans)
    {
        x += get<2>(t);
    }

    return x;
}

void parse_galaxies(vector<string> lines, vector<size_t> &expanded_columns, vector<size_t> &expanded_rows, vector<pair<size_t, size_t>> &galaxies)
{
    for (size_t y = 0; y < lines.size(); ++y)
    {
        expanded_columns.push_back(y);
    }
    for (size_t x = 0; x < lines[0].size(); ++x)
    {
        expanded_rows.push_back(x);
    }
    for (size_t y = 0; y < lines.size(); ++y)
    {
        for (size_t x = 0; x < lines[y].size(); ++x)
        {
            if (lines[y][x] == '#')
            {
                galaxies.push_back({x, y});
                if (find(expanded_columns.begin(), expanded_columns.end(), y) != expanded_columns.end())
                {
                    expanded_columns.erase(find(expanded_columns.begin(), expanded_columns.end(), y));
                }
                if ((find(expanded_rows.begin(), expanded_rows.end(), x) != expanded_rows.end()))
                {
                    expanded_rows.erase(find(expanded_rows.begin(), expanded_rows.end(), x));
                }
            }
        }
    }
}

vector<tuple<pair<size_t, size_t>, pair<size_t, size_t>, long long>> calculate_distances(vector<size_t> expanded_columns, vector<size_t> expanded_rows, vector<pair<size_t, size_t>> galaxies,
                                                                                         size_t expansion_coefficient)
{
    vector<tuple<pair<size_t, size_t>, pair<size_t, size_t>, long long>> distances;

    for (size_t i = 0; i < galaxies.size() - 1; ++i)
    {
        pair<size_t, size_t> cur = galaxies[i];
        for (size_t j = i + 1; j < galaxies.size(); ++j)
        {
            pair<size_t, size_t> other = galaxies[j];
            long long exp_c_cross = 0;
            for (auto c : expanded_columns)
            {
                if ((cur.second < c && c < other.second) || (other.second < c && c < cur.second))
                {
                    exp_c_cross++;
                }
            }
            long long exp_r_cross = 0;
            for (auto r : expanded_rows)
            {
                if ((cur.first < r && r < other.first) || (other.first < r && r < cur.first))
                {
                    exp_r_cross++;
                }
            }
            // Calc dist
            distances.push_back({cur, other, (llabs(cur.first - other.first) + (expansion_coefficient - 1) * exp_r_cross) + (llabs(cur.second - other.second) + (expansion_coefficient - 1) * exp_c_cross)});
        }
    }
    // int a = 0;
    // for (size_t i = 0; i < 50; i++)
    // {
    //     auto t = distances[i];
    //     cout << "(" << get<0>(t).first << ", " << get<0>(t).second << ") -> (" << get<1>(t).first << ", " << get<1>(t).second << ") : " << get<2>(t) << endl;
    //     a++;
    // }
    // cout << a << endl;
    return distances;
}
