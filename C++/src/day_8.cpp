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
long long traverse(map<string, shared_ptr<node>>, string, string, string);
vector<tuple<string, long long, long long>> loop_dist(map<string, shared_ptr<node>>, string, char, char);

long long task_1(map<string, shared_ptr<node>>, string);
long long task_2(map<string, shared_ptr<node>>, string);
void cleanup(map<string, shared_ptr<node>> &);

pair<long long, long long> aoc23::day_8()
{
    vector<string> lines = read_file("resources/input_8.txt", true);

    map<string, shared_ptr<node>> nodes = parse_nodes(lines);
    string path = lines[0];

    long long t1a = task_1(nodes, path);

    long long t2a = task_2(nodes, path);

    cleanup(nodes);

    return { t1a, t2a };
}

long long task_1(map<string, shared_ptr<node>> nodes, string path)
{
    long long x = traverse(nodes, path, "AAA", "ZZZ");

    return x;
}

long long task_2(map<string, shared_ptr<node>> nodes, string path)
{
    vector<tuple<string, long long, long long>> loops = loop_dist(nodes, path, 'A', 'Z');

    vector<tuple<string, long long, long long>>::iterator mx = max_element(loops.begin(), loops.end(),
    [](tuple<string, long long, long long> a, tuple<string, long long, long long> b)
    {
        return get<1>(a) < get<1>(b);
    });

    long long total = accumulate(loops.begin(), loops.end(), 1LL,
    [](long long a, tuple<string, long long, long long> b)
    {
        return lcm(a, get<2>(b));
    });

    return total;
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
        kv.second->left.reset();
        kv.second->right.reset();
    }
    nodes.clear();
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
