#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

vector<map<string, vector<int>>> parse_games(std::vector<std::string>);
int valid_games(vector<map<string, vector<int>>>, map<string, int>);
int power(vector<map<string, vector<int>>>);

pair<long long, long long> aoc23::day_2()
{
    std::vector<std::string> lines = read_file("resources/input_2.txt", false);
    vector<map<string, vector<int>>> games = parse_games(lines);

    // Task 1
    map<string, int> limit({{"red", 12}, {"green", 13}, {"blue", 14}});
    int id_sum = valid_games(games, limit);

    // Task 2
    int power_sum = power(games);

    return {id_sum, power_sum};
}

vector<map<string, vector<int>>> parse_games(vector<string> lines)
{
    // Initialise map
    vector<map<string, vector<int>>> game_map;
    for (string line : lines)
    {
        map<string, vector<int>> game;
        for (string turn : split_string(split_string(line, ": ")[1], "; "))
        {
            for (string col : split_string(turn, ", "))
            {
                vector<string> num_col = split_string(col, " ");
                game[num_col[1]].push_back(stoi(num_col[0]));
            }
        }
        game_map.push_back(game);
    }
    return game_map;
}

int power(vector<map<string, vector<int>>> games)
{
    int x = 0;
    for (size_t i = 0; i < games.size(); ++i)
    {
        auto game = games[i];
        int max_red = game.find("red") != game.end() ? *max_element(game["red"].begin(), game["red"].end()) : 1;
        int max_blue = game.find("blue") != game.end() ? *max_element(game["blue"].begin(), game["blue"].end()) : 1;
        int max_green = game.find("green") != game.end() ? *max_element(game["green"].begin(), game["green"].end()) : 1;
        x += (max_red * max_blue * max_green);
    }
    return x;
}

int valid_games(vector<map<string, vector<int>>> games, map<string, int> limit)
{
    int x = 0;
    for (size_t i = 0; i < games.size(); ++i)
    {
        auto game = games[i];
        bool valid = true;
        for (auto &kv : limit)
        {
            if (game.find(kv.first) != game.end() && *max_element(game[kv.first].begin(), game[kv.first].end()) > kv.second)
            {
                valid = false;
            }
        }
        x += (valid ? i + 1 : 0);
    }
    return x;
}
