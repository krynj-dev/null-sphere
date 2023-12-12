#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

vector<int> matching_numbers(string);
int score(vector<int>);
vector<int> parse_numbers(string);
int cards_count(vector<vector<int>>);

pair<long long, long long> aoc23::day_4()
{
    vector<string> lines = read_file("resources/input_4.txt", true);

    vector<vector<int>> game_matches(lines.size());

    for (size_t i = 0; i < lines.size(); i++)
    {
        game_matches[i] = matching_numbers(lines[i]);
    }

    // Task 1
    int score_total = 0;
    for (vector<int> matches: game_matches)
    {
        score_total += score(matches);
    }

    // Task 2
    int x = cards_count(game_matches);

    return { score_total, x };

}

vector<int> matching_numbers(string game_line)
{
    vector<int> matching_numbers;
    map<int, bool> winning_number_map;
    // Split game line in to game and numbers
    vector<string> game_split = split_string(game_line, ":");
    // Split numbers into scratched and winners
    vector<string> number_split = split_string(game_split[1], " |");
    // Get numbers that are had
    vector<long long> have_numbers = aoc23::parse_numbers(number_split[1]);
    // Get winning numbers
    vector<long long> winning_numbers = aoc23::parse_numbers(number_split[0]);
    // Map winning numbers
    for (int n: winning_numbers)
    {
        winning_number_map[n] = true;
    }
    for (int i: have_numbers)
    {
        if (winning_number_map[i])
        {
            matching_numbers.push_back(i);
        }
    }
    return matching_numbers;
}

int score(vector<int> matching_numbers)
{
    return matching_numbers.size() > 0 ? (1 << (matching_numbers.size() - 1)) : 0;
}

void card_number(size_t game_no, size_t &card_count, vector<vector<int>> game_matches)
{
    if (game_no >= game_matches.size() || game_matches[game_no].size() == 0)
    {
        return;
    }
}

int cards_count(vector<vector<int>> games)
{
    vector<size_t> cards(games.size());

    for (int i = games.size() - 1; i >= 0; --i)
    {
        cards[i] = 1;
        for (int j = i + 1; j < i + games[i].size() + 1; j++)
        {
            cards[i] += cards[j];
        }
    }

    int total = 0;
    for (int n: cards)
    {
        total += n;
    }
    return total;
}
