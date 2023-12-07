#include <advent.h>
#include <util.h>
#include <iostream>

using namespace std;
using namespace aoc23;

vector<pair<int, int>> parse_races(vector<string>);
void task_1(vector<pair<int, int>>);
void task_2(pair<long long, long long>);
pair<long long, long long> merge_races(vector<pair<int, int>>);
long long get_number_of_results(pair<long long, long long>);

void aoc23::day_6() {
    vector<string> lines = read_file("resources/input_6.txt", true);

    vector<pair<int, int>> races = parse_races(lines);

    task_1(races);

    pair<long long, long long> big_race = merge_races(races);

    task_2(big_race);
}

vector<pair<int, int>> parse_races(vector<string> lines)
{
    vector<int> times; 
    vector<int> distances; 
    vector<pair<int, int>> races;

    for (long long time: parse_numbers(split_string(lines[0], ":")[1]))
    {
        times.push_back(time);
    }
    for (long long distance: parse_numbers(split_string(lines[1], ":")[1]))
    {
        distances.push_back(distance);
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
        races.push_back({ times[i], distances[i] });
    }

    return races;
}

pair<long long, long long> merge_races(vector<pair<int, int>> races)
{
    string time_str = "";
    string distance_str = "";
    for (pair<int, int> race: races)
    {
        time_str += to_string(race.first);
        distance_str += to_string(race.second);
    }
    return { stoll(time_str), stoll(distance_str) };
}

void task_1(vector<pair<int, int>> races)
{
    // Algorithm: parse from start till first successful, mark it
    // parse from end till first successful, mark it: all between the two marked numbers are valid
    int multiplier = 1;

    for (pair<int, int> race: races)
    {
        multiplier *= get_number_of_results(race);
    }

    cout << "\tTask 1: number is " << multiplier << endl;
    
}

void task_2(pair<long long, long long> race)
{
    long long num_res =  get_number_of_results(race);
    cout << "\tTask 2: number is " << num_res << endl;
}

long long get_number_of_results(pair<long long, long long> race)
{
    long long lowest_success;
    long long highest_success;
    long long time = race.first;
    long long distance = race.second;
    for (long long i = 1; i < time; ++i)
    {
        if (i * (time - i) > distance)
        {
            lowest_success = i;
            break;
        }
    }
    for (long long i = time-1; i > 0; --i)
    {
        if (i * (time - i) > distance)
        {
            highest_success = i;
            break;
        }
    }
    return (highest_success - lowest_success + 1);
}
