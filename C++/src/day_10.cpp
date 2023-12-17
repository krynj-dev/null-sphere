#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

pair<size_t, vector<vector<int>>> task_1(vector<string>);
long long task_2(vector<vector<int>>, vector<string>);

pair<size_t, vector<vector<int>>> dist_map(vector<string>);
size_t get_outgoing_dir(size_t, char);
vector<vector<int>> mark_left_right(vector<vector<int>>, vector<string>);
void check_it(vector<vector<int>> &, size_t, size_t, size_t, size_t);
char start_to_pipe(vector<size_t>);
pair<size_t, size_t> cascade_right(vector<vector<int>>&);

pair<long long, long long> aoc23::day_10()
{
    vector<string> lines = read_file("resources/input_10.txt", true);

    auto task_1_answer = task_1(lines);

    long long task_2_answer = task_2(task_1_answer.second, lines);

    return {task_1_answer.first, task_2_answer};
}

pair<size_t, vector<vector<int>>> task_1(vector<string> lines)
{
    pair<size_t, vector<vector<int>>> distances = dist_map(lines);

    return distances;
}

long long task_2(vector<vector<int>> distance_map, vector<string> pipe_map)
{
    distance_map = mark_left_right(distance_map, pipe_map);
    auto nums = cascade_right(distance_map);

    // for (int y = 0; y < pipe_map.size(); ++y)
    // {
    //     for (int x = 0; x < pipe_map[y].size(); ++x)
    //     {
    //         if (distance_map[y][x] >= 0)
    //         {
    //             cout << "\033[1;32m";
    //         }
    //         else if (distance_map[y][x] == -3)
    //         {
    //             cout << "\033[1;33m";
    //         }
    //         else if (distance_map[y][x] == -4)
    //         {
    //             cout << "\033[1;34m";
    //         }
    //         cout << pipe_map[y][x];
    //         cout << "\033[0m";
    //     }
    //     cout << endl;
    // }
    // cout << "L: " << nums.first << ", R: " << nums.second << endl;

    return nums.second;
}

pair<size_t, vector<vector<int>>> dist_map(vector<string> pipe_map)
{
    vector<vector<int>> aux;
    // Init all as -1
    vector<int> line(pipe_map[0].size());
    for (size_t i = 0; i < pipe_map[0].size(); i++)
    {
        line[i] = -2;
    }
    for (size_t i = 0; i < pipe_map.size(); i++)
    {
        aux.push_back(line);
    }
    // Find loc of S to begin traversal
    size_t s_x;
    size_t s_y;
    for (size_t y = 0; y < pipe_map.size(); ++y)
    {
        size_t cand_x = pipe_map[y].find('S');
        if (cand_x != string::npos)
        {
            s_x = cand_x;
            s_y = y;
            break;
        }
    }
    // Scan for ways out
    vector<size_t> valid_starts;
    string ups = "|7F";
    if (ups.find(pipe_map[s_y - 1][s_x]) != string::npos)
    {
        valid_starts.push_back(0);
    }
    string rights = "-J7";
    if (rights.find(pipe_map[s_y][s_x + 1]) != string::npos)
    {
        valid_starts.push_back(1);
    }
    string downs = "|JL";
    if (downs.find(pipe_map[s_y + 1][s_x]) != string::npos)
    {
        valid_starts.push_back(2);
    }
    string lefts = "-FL";
    if (lefts.find(pipe_map[s_y][s_x - 1]) != string::npos)
    {
        valid_starts.push_back(3);
    }

    size_t one_x = s_x;
    size_t one_y = s_y;
    size_t one_ib = valid_starts[0];
    size_t two_x = s_x;
    size_t two_y = s_y;
    size_t two_ib = valid_starts[1];
    size_t i = 0;
    bool break_after_next = false;
    while (true)
    {
        // Mark then move
        aux[one_y][one_x] = i;
        if (i != 0)
        {
            one_ib = get_outgoing_dir((one_ib + 2) % 4, pipe_map[one_y][one_x]);
        }
        switch (one_ib)
        {
        case 0:
            one_y--;
            break;
        case 1:
            one_x++;
            break;
        case 2:
            one_y++;
            break;
        case 3:
            one_x--;
            break;
        }
        if (one_x == two_x && one_y == two_y)
        {
            break_after_next = true;
            continue;
        }
        // Repeat for the other direction
        aux[two_y][two_x] = i;
        if (i != 0)
        {
            two_ib = get_outgoing_dir((two_ib + 2) % 4, pipe_map[two_y][two_x]);
        }
        switch (two_ib)
        {
        case 0:
            two_y--;
            break;
        case 1:
            two_x++;
            break;
        case 2:
            two_y++;
            break;
        case 3:
            two_x--;
            break;
        }
        i++;
        if (break_after_next)
        {
            break;
        }
        if (one_x == two_x && one_y == two_y)
        {
            break_after_next = true;
        }
    }
    return {i - 1, aux};
}

/*
      - 0 -
      3 X 1
      - 2 -
*/
size_t get_outgoing_dir(size_t inbound_dir, char symbol)
{
    switch (symbol)
    {
    case '|':
    case '-':
        return (inbound_dir + 2) % 4;
    case 'F':
        return (inbound_dir << 1) % 3;
    case 'L':
        return (inbound_dir + 1) % 2;
    case 'J':
        return (inbound_dir + 3) % 6;
    case '7':
        return ((inbound_dir << 1) % 3) + 2;
    }
}

vector<vector<int>> mark_left_right(vector<vector<int>> distance_map, vector<string> pipe_map)
{
    size_t count = 0;
    // Find loc of S to begin traversal
    size_t s_x;
    size_t s_y;
    for (size_t y = 0; y < pipe_map.size(); ++y)
    {
        size_t cand_x = pipe_map[y].find('S');
        if (cand_x != string::npos)
        {
            s_x = cand_x;
            s_y = y;
            break;
        }
    }
    // Scan for ways out
    vector<size_t> valid_starts;
    string ups = "|7F";
    if (ups.find(pipe_map[s_y - 1][s_x]) != string::npos)
    {
        valid_starts.push_back(0);
    }
    string rights = "-J7";
    if (rights.find(pipe_map[s_y][s_x + 1]) != string::npos)
    {
        valid_starts.push_back(1);
    }
    string downs = "|JL";
    if (downs.find(pipe_map[s_y + 1][s_x]) != string::npos)
    {
        valid_starts.push_back(2);
    }
    string lefts = "-FL";
    if (lefts.find(pipe_map[s_y][s_x - 1]) != string::npos)
    {
        valid_starts.push_back(3);
    }

    size_t x = s_x;
    size_t y = s_y;
    size_t to = (valid_starts[0] + 2) % 4;
    size_t i = 0;
    do
    {
        char pipe_type = pipe_map[y][x] == 'S' ? start_to_pipe(valid_starts) : pipe_map[y][x];
        size_t from = (to + 2) % 4;
        to = get_outgoing_dir(from, pipe_type);
        check_it(distance_map, x, y, from, to);
        switch (to)
        {
        case 0:
            y--;
            break;
        case 1:
            x++;
            break;
        case 2:
            y++;
            break;
        case 3:
            x--;
            break;
        }
        i++;
    } while (pipe_map[y][x] != 'S');
    return distance_map;
}

char start_to_pipe(vector<size_t> valid_outbound)
{
    bool has_zero = find(valid_outbound.begin(), valid_outbound.end(), 0) != valid_outbound.end();
    bool has_one = find(valid_outbound.begin(), valid_outbound.end(), 1) != valid_outbound.end();
    bool has_two = find(valid_outbound.begin(), valid_outbound.end(), 2) != valid_outbound.end();
    bool has_three = find(valid_outbound.begin(), valid_outbound.end(), 3) != valid_outbound.end();
    if (has_zero && has_one)
    {
        return 'L';
    }
    else if (has_zero && has_two)
    {
        return '|';
    }
    else if (has_zero && has_three)
    {
        return 'J';
    }
    else if (has_one && has_two)
    {
        return 'F';
    }
    else if (has_one && has_three)
    {
        return '-';
    }
    else
    {
        return '7';
    }
}

// Right symbol = -3, Left symbol = -4
void check_it(vector<vector<int>> &distance_map, size_t x, size_t y, size_t from, size_t to)
{
    int dir = (1 << from) + (1 << to);
    int flow = to - from;
    if (dir == 3) // to/from = 0/1 = L
    {
        if (y + 1 < distance_map.size() && distance_map[y + 1][x] < 0)
        {
            distance_map[y + 1][x] = (flow < 0) ? -3 : -4;
        }
        if (x - 1 >= 0 && distance_map[y][x - 1] < 0)
        {
            distance_map[y][x - 1] = (flow < 0) ? -3 : -4;
        }
    }
    else if (dir == 5) // to/from = 0/2 - |
    {
        if (x - 1 >= 0 && distance_map[y][x - 1] < 0)
        {
            distance_map[y][x - 1] = (flow < 0) ? -3 : -4;
        }
        if (x + 1 < distance_map[y].size() && distance_map[y][x + 1] < 0)
        {
            distance_map[y][x + 1] = (flow < 0) ? -4 : -3;
        }
    }
    else if (dir == 6) // to/from = 1/2 - F
    {
        if (x - 1 >= 0 && distance_map[y][x - 1] < 0)
        {
            distance_map[y][x - 1] = (flow < 0) ? -3 : -4;
        }
        if (y - 1 >= 0 && distance_map[y - 1][x] < 0)
        {
            distance_map[y - 1][x] = (flow < 0) ? -3 : -4;
        }
    }
    else if (dir == 9) // to/from = 0/3 - J
    {
        if (x + 1 < distance_map[y].size() && distance_map[y][x + 1] < 0)
        {
            distance_map[y][x + 1] = (flow < 0) ? -4 : -3;
        }
        if (y + 1 < distance_map.size() && distance_map[y + 1][x] < 0)
        {
            distance_map[y + 1][x] = (flow < 0) ? -4 : -3;
        }
    }
    else if (dir == 10) // to/from = 1/3 - -
    {
        if (y - 1 >= 0 && distance_map[y - 1][x] < 0)
        {
            distance_map[y - 1][x] = (flow < 0) ? -3 : -4;
        }
        if (y + 1 < distance_map.size() && distance_map[y + 1][x] < 0)
        {
            distance_map[y + 1][x] = (flow < 0) ? -4 : -3;
        }
    }
    else if (dir == 12) // to/from = 2/3 - 7
    {
        if (y - 1 >= 0 && distance_map[y - 1][x] < 0)
        {
            distance_map[y - 1][x] = (flow < 0) ? -3 : -4;
        }
        if (x + 1 < distance_map.size() && distance_map[y][x + 1] < 0)
        {
            distance_map[y][x + 1] = (flow < 0) ? -3 : -4;
        }
    }
}

pair<size_t, size_t> cascade_right(vector<vector<int>> &distance_map)
{
    // Init visit map
    vector<vector<bool>> visited;
    vector<bool> line(false, distance_map[0].size());
    for (size_t i = 0; i < distance_map[0].size(); i++)
    {
        line.push_back(false);
    }
    for (size_t i = 0; i < distance_map.size(); i++)
    {
        visited.push_back(line);
    }
    // Init queue and add all marked L/R to it
    queue<pair<size_t, size_t>> l_queue;
    queue<pair<size_t, size_t>> r_queue;
    for (size_t y = 0; y < distance_map.size(); ++y)
    {
        for (size_t x = 0; x < distance_map[y].size(); ++x)
        {
            if (distance_map[y][x] == -3)
            {
                l_queue.push({x, y});
                visited[y][x] = true;
            }
            if (distance_map[y][x] == -4)
            {
                r_queue.push({x, y});
                visited[y][x] = true;
            }
        }
    }
    // Go through queues, maintain count
    int x;
    int y;
    size_t l_count = 0;
    while (!l_queue.empty())
    {
        // Pop from queue
        pair<int, int> cur = l_queue.front();
        l_queue.pop();
        x = cur.first;
        y = cur.second;
        // Add unvisited neighbours to queue
        bool pushed = false;
        if (x - 1 >= 0 && !visited[y][x - 1] && distance_map[y][x - 1] == -2)
        {
            l_queue.push({x - 1, y});
            visited[y][x - 1] = true;
        }
        if (x + 1 < distance_map[y].size() && !visited[y][x + 1] && distance_map[y][x + 1] == -2)
        {
            l_queue.push({x + 1, y});
            visited[y][x + 1] = true;
        }
        if (y - 1 >= 0 && !visited[y - 1][x] && distance_map[y - 1][x] == -2)
        {
            l_queue.push({x, y - 1});
            visited[y - 1][x] = true;
        }
        if (y + 1 < distance_map.size() && !visited[y + 1][x] && distance_map[y + 1][x] == -2)
        {
            l_queue.push({x, y + 1});
            visited[y + 1][x] = true;
        }
        
        distance_map[y][x] = -3;
        l_count++;
    }
    // Repeat for right
    size_t r_count = 0;
    while (!r_queue.empty())
    {
        // Pop from queue
        pair<int, int> cur = r_queue.front();
        r_queue.pop();
        x = cur.first;
        y = cur.second;
        // Add unvisited neighbours to queue
        bool pushed = false;
        if (x - 1 >= 0 && !visited[y][x - 1] && distance_map[y][x - 1] == -2)
        {
            r_queue.push({x - 1, y});
            visited[y][x - 1] = true;
        }
        if (x + 1 < distance_map[y].size() && !visited[y][x + 1] && distance_map[y][x + 1] == -2)
        {
            r_queue.push({x + 1, y});
            visited[y][x + 1] = true;
        }
        if (y - 1 >= 0 && !visited[y - 1][x] && distance_map[y - 1][x] == -2)
        {
            r_queue.push({x, y - 1});
            visited[y - 1][x] = true;
        }
        if (y + 1 < distance_map.size() && !visited[y + 1][x] && distance_map[y + 1][x] == -2)
        {
            r_queue.push({x, y + 1});
            visited[y + 1][x] = true;
        }
        
        distance_map[y][x] = -4;
        r_count++;
    }
    return { l_count, r_count };
}
