#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

namespace day_16
{
    long long task_1(vector<string>);
    long long task_2(vector<string>);
}

long long beam_traverse(vector<string>, int, int, size_t);
long long recur_beam_traverse(int, int, size_t, vector<string> &,
                              vector<vector<map<size_t, long long>>> &, vector<vector<map<size_t, bool>>> &);

pair<long long, long long> aoc23::day_16()
{
    vector<string> lines = read_file("resources/input_16.txt", true);

    long long task_1_answer = day_16::task_1(lines);
    
    long long task_2_answer = day_16::task_2(lines);
    
    return {task_1_answer, task_2_answer};
}

long long day_16::task_1(vector<string> lines)
{
    return beam_traverse(lines, 0, 0, 1);
}

long long day_16::task_2(vector<string> lines)
{

    map<tuple<size_t, size_t, size_t>, long long> energises;
    for (size_t y = 0; y < lines.size(); ++y)
    {
        energises[{0, y, 1}] = beam_traverse(lines, 0, y, 1);
        energises[{lines[0].size() - 1, y, 3}] = beam_traverse(lines, lines[0].size() - 1, y, 3);
    }
    for (size_t x = 0; x < lines[0].size(); ++x)
    {
        energises[{x, 0, 2}] = beam_traverse(lines, x, 0, 2);
        energises[{x, lines.size() - 1, 0}] = beam_traverse(lines, x, lines.size() - 1, 0);
    }

    return (max_element(energises.begin(), energises.end(),
                       [](pair<tuple<size_t, size_t, size_t>, long long> a, pair<tuple<size_t, size_t, size_t>, long long> b)
                       {
                            return a.second < b.second;
                       }))->second;
}

long long beam_traverse(vector<string> lines, int xi, int yi, size_t diri)
{
    long long energised = 0;
    // x, y, dir. Dir: 0-up, 1-right, 2-down, 3-left
    queue<tuple<int, int, size_t>> beam_q;
    vector<vector<map<size_t, bool>>> visited;
    for (auto s : lines)
    {
        vector<map<size_t, bool>> temp(s.size());
        visited.push_back(temp);
    }
    beam_q.push({xi, yi, diri});
    // begin loop
    while (!beam_q.empty())
    {
        tuple<int, int, size_t> tile = beam_q.front();
        beam_q.pop();
        int x = get<0>(tile);
        int y = get<1>(tile);
        size_t dir = get<2>(tile);
        // cout << "DEBUG: " << x << ", " << y << ": " << dir << endl;
        if (x < 0 || x >= lines[0].size() || y < 0 || y >= lines.size() ||
            (visited[y][x].find(dir) != visited[y][x].end() && visited[y][x][dir]))
        {
            continue;
        }
        if (visited[y][x].size() == 0)
        {
            ++energised;
        }
        visited[y][x][dir] = true;
        char tile_type = lines[y][x];
        if (tile_type == '.' ||
            (tile_type == '|' && (dir == 0 || dir == 2)) ||
            (tile_type == '-' && (dir == 1 || dir == 3)))
        {
            switch (dir)
            {
            case 0:
                beam_q.push({x, y - 1, 0});
                break;
            case 1:
                beam_q.push({x + 1, y, 1});
                break;
            case 2:
                beam_q.push({x, y + 1, 2});
                break;
            case 3:
                beam_q.push({x - 1, y, 3});
                break;
            default:
                break;
            }
        }
        else if (tile_type == '\\')
        {
            switch (dir)
            {
            case 0:
                beam_q.push({x - 1, y, 3});
                break;
            case 1:
                beam_q.push({x, y + 1, 2});
                break;
            case 2:
                beam_q.push({x + 1, y, 1});
                break;
            case 3:
                beam_q.push({x, y - 1, 0});
                break;
            default:
                break;
            }
        }
        else if (tile_type == '/')
        {
            switch (dir)
            {
            case 0:
                beam_q.push({x + 1, y, 1});
                break;
            case 1:
                beam_q.push({x, y - 1, 0});
                break;
            case 2:
                beam_q.push({x - 1, y, 3});
                break;
            case 3:
                beam_q.push({x, y + 1, 2});
                break;
            default:
                break;
            }
        }
        else if (tile_type == '|')
        {
            beam_q.push({x, y - 1, 0});
            beam_q.push({x, y + 1, 2});
        }
        else if (tile_type == '-')
        {
            beam_q.push({x - 1, y, 3});
            beam_q.push({x + 1, y, 1});
        }
    }

    return energised;
}
