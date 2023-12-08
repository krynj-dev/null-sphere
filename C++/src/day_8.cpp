#include <advent.h>
#include <util.h>
#include <climits>
#include <numeric>

using namespace std;
using namespace aoc23;

class node;

class node
{
public:
    string name;
    shared_ptr<node> left;
    shared_ptr<node> right;
    node(string name)
    {
        this->name = name;
    }
};

map<string, shared_ptr<node>> parse_nodes(vector<string>);
void print_map(std::map<std::string, std::shared_ptr<node>> &);
long long traverse(map<string, shared_ptr<node>>, string, string, string);
long long parallel_traverse(map<string, shared_ptr<node>>, string, char, char);
vector<tuple<string, long long, long long>> loop_dist(map<string, shared_ptr<node>>, string, char, char);
long long calc_z(map<string, shared_ptr<node>>, string, tuple<string, long long, long long>);

void task_1(map<string, shared_ptr<node>>, string);
void task_2(map<string, shared_ptr<node>>, string);
void cleanup(map<string, shared_ptr<node>> &);

void aoc23::day_8()
{
    vector<string> lines = read_file("resources/input_8.txt", true);

    map<string, shared_ptr<node>> nodes = parse_nodes(lines);
    string path = lines[0];

    task_1(nodes, path);

    task_2(nodes, path);

    cleanup(nodes);
}

void task_1(map<string, shared_ptr<node>> nodes, string path)
{
    // Put stuff here

    long long x = traverse(nodes, path, "AAA", "ZZZ");

    cout << "\tTask 1: " << x << endl;
}

void task_2(map<string, shared_ptr<node>> nodes, string path)
{
    // Put stuff here
    // long long x = parallel_traverse(nodes, path, 'A', 'Z');
    vector<tuple<string, long long, long long>> loops = loop_dist(nodes, path, 'A', 'Z');

    vector<long long> adjusted;

    vector<tuple<string, long long, long long>>::iterator mx = max_element(loops.begin(), loops.end(),
    [](tuple<string, long long, long long> a, tuple<string, long long, long long> b)
    {
        return get<1>(a) < get<1>(b);
    });
    // cout << "MAX: " << get<1>(*mx) << endl;

    for (tuple<string, long long, long long> loop : loops)
    {
        // cout << "(" << get<0>(loop) << ": " << get<1>(loop) << ", " << get<2>(loop) << ")" << " ";
        long long z = calc_z(nodes, path, loop);
        adjusted.push_back(z - (get<1>(*mx) - get<1>(loop)));
        // cout << z << " --> " << z - (get<1>(*mx) - get<1>(loop)) << " ";
    }
    // cout << endl;

    long long total = accumulate(loops.begin(), loops.end(), 1LL,
    [](long long a, tuple<string, long long, long long> b)
    {
        return lcm(a, get<2>(b));
    });

    cout << "\tTask 2: " << total << endl;
    // cout << x << endl;
}

map<string, shared_ptr<node>> parse_nodes(vector<string> lines)
{
    map<string, shared_ptr<node>> nodes;
    for (string line : lines)
    {
        if (line.find("=") != string::npos)
        {
            vector<string> components = split_string(line, " = ");
            // Create empty nodes
            nodes[components[0]] = make_shared<node>(components[0]);
        }
    }
    for (string line : lines)
    {
        if (line.find("=") != string::npos)
        {
            vector<string> components = split_string(line, " = ");
            // Fill nodes
            vector<string> children_components = split_string(components[1], ", ");
            auto &parent = nodes[components[0]];
            parent->left = nodes[children_components[0].substr(1)];
            parent->right = nodes[children_components[1].substr(0, 3)];
        }
    }
    return nodes;
}

void cleanup(map<string, shared_ptr<node>> &nodes)
{
    for (auto &kv : nodes)
    {
        // Clear the shared_ptr members before clearing the map
        kv.second->left.reset();
        kv.second->right.reset();
    }
    nodes.clear();
}

void print_map(std::map<std::string, std::shared_ptr<node>> &nodes)
{
    for (const auto &entry : nodes)
    {
        const auto &node = entry.second;
        if (node)
        {
            std::string left_name = (node->left) ? node->left->name : "null";
            std::string right_name = (node->right) ? node->right->name : "null";

            printf("%s: (%s, %s)\n", entry.first.c_str(), left_name.c_str(), right_name.c_str());
        }
        else
        {
            printf("%s: (null, null)\n", entry.first.c_str());
        }
    }
}

long long traverse(map<string, shared_ptr<node>> nodes, string path, string start, string end)
{
    long long i = 0;
    shared_ptr<node> cur_node = nodes[start];
    while (i < LLONG_MAX)
    {
        if (cur_node->name == end)
        {
            break;
        }
        char dir = path[i % path.size()];
        switch (dir)
        {
        case 'L':
            cur_node = cur_node->left;
            break;
        case 'R':
            cur_node = cur_node->right;
            break;
        }
        i++;
    }
    return i;
}

long long parallel_traverse(map<string, shared_ptr<node>> nodes, string path, char start, char end)
{
    long long i = 0;
    vector<shared_ptr<node>> cur_nodes;
    int f = 0;
    for (map<string, shared_ptr<node>>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        if (it->first[2] == start)
        {
            cur_nodes.push_back(it->second);
            f++;
            if (f == 2)
            {
                break;
            }
        }
    }
    while (i < LLONG_MAX)
    {
        bool all_ended = true;
        for (shared_ptr<node> cur_node : cur_nodes)
        {
            if (i % 100000 == 0)
            {
                cout << cur_node->name << " ";
            }
            if (cur_node->name[2] != end)
            {
                all_ended = false;
            }
        }
        if (all_ended)
        {
            break;
        }
        if (i % 100000 == 0)
        {
            cout << endl;
        }
        char dir = path[i % path.size()];
        for (size_t j = 0; j < cur_nodes.size(); ++j)
        {
            shared_ptr<node> cur_node = cur_nodes[j];
            switch (dir)
            {
            case 'L':
                cur_nodes[j] = cur_node->left;
                break;
            case 'R':
                cur_nodes[j] = cur_node->right;
                break;
            }
        }
        i++;
    }
    return i;
}

vector<tuple<string, long long, long long>> loop_dist(map<string, shared_ptr<node>> nodes, string path, char start, char end)
{
    vector<shared_ptr<node>> cur_nodes;
    vector<tuple<string, long long, long long>> dists;
    for (map<string, shared_ptr<node>>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        if (it->first[2] == start)
        {
            cur_nodes.push_back(it->second);
        }
    }
    for (size_t j = 0; j < cur_nodes.size(); ++j)
    {
        string start_name = cur_nodes[j]->name;
        shared_ptr<node> cur_node = cur_nodes[j];
        map<string, vector<pair<long long, long long>>> seens;
        long long i = 0;
        tuple<string, long long, long long> answer;
        while (i < LLONG_MAX)
        {
            auto it = find_if(seens[cur_node->name].begin(), seens[cur_node->name].end(),
                              [i, path](pair<long long, long long> a)
                              {
                                  return a.first == i % path.size();
                              });
            if ((seens.find(cur_node->name) != seens.end()) && it != seens[cur_node->name].end())
            {
                // cout << cur_node->name << " repeated at path[" << i % path.size() << "] @" << it->second << " and @" << i << endl;
                dists.push_back({cur_node->name, i % path.size(), i - it->second});
                break;
            }
            seens[cur_node->name].push_back({i % path.size(), i});
            char dir = path[i % path.size()];
            switch (dir)
            {
            case 'L':
                cur_node = cur_node->left;
                break;
            case 'R':
                cur_node = cur_node->right;
                break;
            }
            i++;
        }
    }
    return dists;
}

long long calc_z(map<string, shared_ptr<node>> nodes, string path, tuple<string, long long, long long> loop)
{
    shared_ptr<node> cur_node = nodes[get<0>(loop)];
    long long x = 0;
    while (x < get<2>(loop))
    {
        if (cur_node->name[2] == 'Z')
        {
            return x;
        }
        char dir = path[(x + get<1>(loop)) % path.size()];
        switch (dir)
        {
        case 'L':
            cur_node = cur_node->left;
            break;
        case 'R':
            cur_node = cur_node->right;
            break;
        }
        x++;
    }
    return -1;
}
