#include <advent.h>
#include <util.h>
#include <iostream>
#include <map>

using namespace std;
using namespace aoc23;

std::map<std::string, std::vector<std::map<std::string, int>>> parse_games(std::vector<std::string> lines);
std::vector<string> valid_games(std::map<std::string, std::vector<std::map<std::string, int>>> games,
                                std::map<std::string, int> colour_limits);

void aoc23::day_2()
{
    std::vector<std::string> lines = read_file("resources/input_2.txt");
    map<string, vector<map<string, int>>> games = parse_games(lines);
    // Task 1
    map<string, int> limit({
        {"red", 12},
        {"green", 13},
        {"blue", 14}
    });
    vector<string> valid = valid_games(games, limit);
    int id_sum = 0;
    for (vector<string>::iterator it = valid.begin(); it != valid.end(); it++) {
        id_sum += stoi(split_string(*it, " ")[1]);
    }
    cout << "Task 1: total is " << id_sum << endl;
    // Task 2
}

std::map<std::string, std::vector<std::map<std::string, int>>> parse_games(std::vector<std::string> lines)
{
    // Initialise map
    std::map<std::string, std::vector<std::map<std::string, int>>> game_map;
    for (vector<string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        // Split Game name from turns
        vector<string> game_split = split_string(*it, ": ");
        // Split turns
        vector<string> turn_split = split_string(game_split[1], "; ");
        // Map turns
        std::vector<std::map<std::string, int>> game_turns;
        for (vector<string>::iterator turn_it = turn_split.begin(); turn_it != turn_split.end(); turn_it++)
        {
            map<string, int> turn_map;
            vector<string> colour_split = split_string(*turn_it, ", ");
            for (vector<string>::iterator colour_it = colour_split.begin(); colour_it != colour_split.end(); colour_it++)
            {
                vector<string> count_split = split_string(*colour_it, " ");
                if (turn_map.find(count_split[1]) != turn_map.end())
                {
                    turn_map.insert({count_split[1], stoi(count_split[0])});
                }
                else
                {
                    turn_map[count_split[1]] = turn_map[count_split[1]] + stoi(count_split[0]);
                }
            }
            game_turns.push_back(turn_map);
        }
        game_map.insert({game_split[0], game_turns});
    }
    return game_map;
}

std::vector<string> valid_games(std::map<std::string, std::vector<std::map<std::string, int>>> games,
                                std::map<std::string, int> colour_limits)
{
    std::vector<string> valid_vector;
    for (map<string, vector<map<string, int>>>::iterator it = games.begin(); it != games.end(); it++)
    {
        bool valid = true;
        vector<map<string, int>> turns = it->second;
        for (vector<map<string, int>>::iterator turn_it = turns.begin(); turn_it != turns.end(); turn_it++)
        {
            map<string, int> turn = *turn_it;
            for (map<string, int>::iterator limit_it = colour_limits.begin(); limit_it != colour_limits.end(); limit_it++) {
                if (turn.find(limit_it->first) != turn.end() && turn[limit_it->first] > limit_it->second) {
                    valid = false;
                }
            }
        }
        if (valid) {
            valid_vector.push_back(it->first);
        }
    }
    return valid_vector;
}
