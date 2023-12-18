#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

namespace day_14
{
    long long task_1(vector<string>);
    long long task_2(vector<string>);
}

long long calc_load(vector<string>);
vector<string> do_cycle(vector<string>, map<string, string> &, int &);
string slide_circles(string);
long long load_no_shove(vector<string>);

pair<long long, long long> aoc23::day_14()
{
    vector<string> lines = read_file("resources/input_14.txt", true);

    long long task_1_answer = day_14::task_1(lines);

    long long task_2_answer = day_14::task_2(lines);

    return {task_1_answer, task_2_answer};
}

long long day_14::task_1(vector<string> lines)
{
    return calc_load(lines);
}

long long day_14::task_2(vector<string> lines)
{
    map<string, string> memo;
    vector<string> new_lines = lines;
    map<vector<string>, vector<string>> memo2;
    map<vector<string>, vector<size_t>> memo3;
    int hit_c = 0;
    size_t loop_len;
    for (size_t i = 0; i < 1000000; ++i)
    {
        if (memo2.find(new_lines) != memo2.end())
        {
            new_lines = memo2[new_lines];
            continue;
        }
        vector<string> cycl_lines = do_cycle(new_lines, memo, hit_c);
        memo2[new_lines] = cycl_lines;
        memo3[cycl_lines].push_back(i);
        new_lines = cycl_lines;
        if (memo3[new_lines].size() > 1)
        {
            loop_len = memo3[new_lines][1] - memo3[new_lines][0];
            break;
        }
    }
    size_t cycle_count = (1000000000 - memo3[new_lines][0]) % loop_len;

    for (size_t i = 0; i < cycle_count-1; ++i)
    {
        new_lines = memo2[new_lines];
    }

    return load_no_shove(new_lines);
}

long long load_no_shove(vector<string> lines)
{
    long long x = 0;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        x += (count(lines[i].begin(), lines[i].end(), 'O') * (lines.size()-i));
    }
    return x;
}

long long calc_load(vector<string> lines)
{
    long long load = 0;
    for (size_t x = 0; x < lines[0].size(); ++x)
    {
        int last_hash = -1;
        int circle_count = 0;
        for (size_t y = 0; y < lines.size(); ++y)
        {
            if (lines[y][x] == '#')
            {
                last_hash = y;
                circle_count = 0;
            }
            else if (lines[y][x] == 'O')
            {
                load += (lines.size() - last_hash - 1 - circle_count);
                ++circle_count;
            }
        }
    }
    return load;
}

vector<string> do_cycle(vector<string> lines, map<string, string> &memo, int &hit_count)
{
    vector<string> new_lines = lines;
    // North
    for (size_t i = 0; i < new_lines[0].size(); ++i)
    {
        string row;
        for (vector<string>::reverse_iterator it = new_lines.rbegin(); it != new_lines.rend(); ++it)
            row += (*it)[i];

        if (memo.find(row) != memo.end())
        {
            row = memo[row];
            hit_count++;
        }
        else
        {
            string new_row = slide_circles(row);
            memo[row] = new_row;
            row = new_row;
        }
        for (int j = new_lines.size() - 1; j >= 0; --j)
        {
            new_lines[j][i] = row[new_lines.size() - j - 1];
        }
    }
    // West
    for (size_t i = 0; i < new_lines.size(); ++i)
    {
        string row = "";
        for (int it = new_lines[i].size() - 1; it >= 0; --it)
        {
            row += new_lines[i][it];
            hit_count++;
        }

        if (memo.find(row) != memo.end())
        {
            row = memo[row];
        }
        else
        {
            string new_row = slide_circles(row);
            memo[row] = new_row;
            row = new_row;
        }
        reverse(row.begin(), row.end());
        new_lines[i] = row;
    }
    // South
    for (size_t i = 0; i < new_lines[0].size(); ++i)
    {
        string row;
        for (vector<string>::iterator it = new_lines.begin(); it != new_lines.end(); ++it)
            row += (*it)[i];

        if (memo.find(row) != memo.end())
        {
            row = memo[row];
            hit_count++;
        }
        else
        {
            string new_row = slide_circles(row);
            memo[row] = new_row;
            row = new_row;
        }
        for (size_t j = 0; j < new_lines.size(); ++j)
        {
            new_lines[j][i] = row[j];
        }
    }
    // East
    for (size_t i = 0; i < new_lines.size(); ++i)
    {
        string row = new_lines[i];

        if (memo.find(row) != memo.end())
        {
            row = memo[row];
            hit_count++;
        }
        else
        {
            string new_row = slide_circles(row);
            memo[row] = new_row;
            row = new_row;
        }
        new_lines[i] = row;
    }
    return new_lines;
}

string slide_circles(string row)
{
    queue<size_t> circle_queue;
    for (size_t i = 0; i < row.size(); ++i)
    {
        if (row[i] == 'O')
        {
            circle_queue.push(i);
        }
        if (row[i] == '#' || i + 1 == row.size())
        {
            int j = (i + 1 == row.size()) ? i : i - 1;
            while (!circle_queue.empty())
            {
                int o_loc = circle_queue.front();
                circle_queue.pop();
                for (; j >= 0; --j)
                {
                    if (row[j] == '.')
                    {
                        if (o_loc < j)
                        {
                            char c = row[j];
                            row[j] = row[o_loc];
                            row[o_loc] = c;
                        }
                        break;
                    }
                }
            }
        }
    }
    return row;
}