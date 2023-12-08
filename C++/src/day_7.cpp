#include <advent.h>
#include <util.h>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace aoc23;

enum Card
{
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE
};

void task_1(vector<pair<vector<Card>, int>>);
void task_2();

vector<pair<vector<Card>, int>> parse_hands(vector<string>);
bool compare_hand(vector<Card>, vector<Card>);
bool compare_hand_bet(pair<vector<Card>, int>, pair<vector<Card>, int>);
Card card_from_char(char);
int hand_type(vector<Card>);
string hand_to_string(vector<Card>);

void aoc23::day_7()
{
    vector<string> lines = read_file("resources/input_7.txt", true);

    vector<pair<vector<Card>, int>> hands = parse_hands(lines);

    task_1(hands);

    task_2();
}

void task_1(vector<pair<vector<Card>, int>> hands)
{
    // Put stuff here

    sort(hands.begin(), hands.end(), compare_hand_bet);

    int total = 0;
    for (size_t i = 0; i < hands.size(); ++i)
    {
        int rank = hands.size() - i;
        pair<vector<Card>, int> hand = hands[i];
        if (i < 100)
        {
            cout << hand_to_string(hand.first) << "\t" << hand.second << '\t' << hand_type(hand.first) << endl;
        }
        
        total += (rank * hand.second);
    }

    cout << "\tTask 1:  " << total << endl;
}

void task_2()
{
    // Put stuff here

    int x = 0;

    cout << "\tTask 2: " << x << endl;
}

vector<pair<vector<Card>, int>> parse_hands(vector<string> lines)
{
    vector<pair<vector<Card>, int>> hand_bets;
    for (string line : lines)
    {
        if (!line.empty())
        {
            vector<string> components = split_string(line, " ");
            vector<Card> hand;
            for (char x : components[0])
            {
                hand.push_back(card_from_char(x));
            }
            hand_bets.push_back({hand, stoi(components[1])});
        }
    }
    return hand_bets;
}

Card card_from_char(char x)
{
    switch (x)
    {
    case '2':
        return TWO;
    case '3':
        return THREE;
    case '4':
        return FOUR;
    case '5':
        return FIVE;
    case '6':
        return SIX;
    case '7':
        return SEVEN;
    case '8':
        return EIGHT;
    case '9':
        return NINE;
    case 'T':
        return TEN;
    case 'J':
        return JACK;
    case 'Q':
        return QUEEN;
    case 'K':
        return KING;
    case 'A':
        return ACE;
    default:
        return TWO;
    }
}

string hand_to_string(vector<Card> hand)
{
    string str = "";
    for (Card c : hand)
    {
        switch (c)
        {
        case TWO:
            str += "2";
            break;
        case THREE:
            str += "3";
            break;
        case FOUR:
            str += "4";
            break;
        case FIVE:
            str += "5";
            break;
        case SIX:
            str += "6";
            break;
        case SEVEN:
            str += "7";
            break;
        case EIGHT:
            str += "8";
            break;
        case NINE:
            str += "9";
            break;
        case TEN:
            str += "T";
            break;
        case JACK:
            str += "J";
            break;
        case QUEEN:
            str += "Q";
            break;
        case KING:
            str += "K";
            break;
        case ACE:
            str += "A";
            break;
        default:
            str += "";
            break;
        }
    }
    return str;
}

bool compare_hand_bet(pair<vector<Card>, int> a, pair<vector<Card>, int> b)
{
    return compare_hand(a.first, b.first);
}

bool compare_hand(vector<Card> a, vector<Card> b)
{
    int type_compare = hand_type(a) - hand_type(b);
    if (type_compare > 0) // i.e. A > B
    {
        return true;
    }
    else if (type_compare < 0) // i.e. A < B
    {
        return false;
    }
    // Compare for highest card l->r
    for (size_t i = 0; i < a.size(); ++i)
    {
        int card_compare = a[i] - b[i];
        // cout << "Comparing cards: " << card_compare << endl;
        if (card_compare > 0) // i.e. A > B
        {
            return true;
        }
        else if (card_compare < 0) // i.e. A < B
        {
            return false;
        }
    }
    return false;
}

// 0 = High, 1 = one pair, 2 = two pair, 3 = three of a kind, 5 = full house, 6 = four kind, 7 = five kind
int hand_type(vector<Card> hand)
{
    int score = 0;
    vector<bool> skip(hand.size());
    for (size_t i = 0; i < hand.size() - 1; ++i)
    {
        if (skip[i])
        {
            continue;
        }
        size_t dupe_count = 0;
        for (size_t j = i + 1; j < hand.size(); j++)
        {
            if (hand[i] == hand[j])
            {
                skip[j] = true;
                dupe_count++;
            }
        }
        switch (dupe_count)
        {
        case 4: // five kind
            return 7;
        case 3: // four kind
            return 6;
        case 2: // three kind
            score += 3;
        case 1: // pair
            score += 1;
        }
    }
    return score;
}
