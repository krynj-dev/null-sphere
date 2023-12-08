#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

enum Card
{
    JOKER,
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

void task(vector<pair<vector<Card>, int>>, bool);

vector<pair<vector<Card>, int>> parse_hands(vector<string>, bool);
bool compare_hand(vector<Card>, vector<Card>, bool);
bool compare_hand_bet(pair<vector<Card>, int>, pair<vector<Card>, int>, bool);
Card card_from_char(char, bool);
int hand_type(vector<Card>, bool);
string hand_to_string(vector<Card>);

void aoc23::day_7()
{
    vector<string> lines = read_file("resources/input_7.txt", true);

    vector<pair<vector<Card>, int>> hands_1 = parse_hands(lines, false);

    task(hands_1, false);

    vector<pair<vector<Card>, int>> hands_2 = parse_hands(lines, true);

    task(hands_2, true);
}

void task(vector<pair<vector<Card>, int>> hands, bool joker)
{
    // Put stuff here

    sort(hands.begin(), hands.end(),
         [joker](pair<vector<Card>, int> a, pair<vector<Card>, int> b)
         {
             return compare_hand_bet(a, b, joker);
         });

    int total = 0;
    for (size_t i = 0; i < hands.size(); ++i)
    {
        int rank = hands.size() - i;
        pair<vector<Card>, int> hand = hands[i];
        // if (i < 100)
        // {
        //     int ht = hand_type(hand.first, joker);
        //     cout << hand_to_string(hand.first) << "\t" << hand.second << '\t' << ht << endl;
        // }

        total += (rank * hand.second);
    }

    cout << "\tTask " << (joker ? "2" : "1") << ":  " << total << endl;
}

vector<pair<vector<Card>, int>> parse_hands(vector<string> lines, bool joker)
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
                hand.push_back(card_from_char(x, joker));
            }
            hand_bets.push_back({hand, stoi(components[1])});
        }
    }
    return hand_bets;
}

Card card_from_char(char x, bool joker)
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
        return joker ? JOKER : JACK;
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
        case JOKER:
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

bool compare_hand_bet(pair<vector<Card>, int> a, pair<vector<Card>, int> b, bool joker)
{
    return compare_hand(a.first, b.first, joker);
}

bool compare_hand(vector<Card> a, vector<Card> b, bool joker)
{
    int type_compare = hand_type(a, joker) - hand_type(b, joker);
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

// 0 = High, 1 = one pair, 2 = two pair, 3 = three of a kind, 4 = full house, 5 = four kind, 6 = five kind
int hand_type(vector<Card> hand, bool joker)
{
    int score = 0;
    vector<bool> skip(hand.size());
    map<Card, size_t> card_counts;
    for (size_t i = 0; i < hand.size(); ++i)
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
        card_counts[hand[i]] = dupe_count + 1;
        switch (dupe_count)
        {
        case 4: // five kind
            score = 6;
            break;
        case 3: // four kind
            score = 5;
            break;
        case 2: // three kind
            score += 3;
            break;
        case 1: // pair
            score += 1;
            break;
        default:
            break;
        }
    }
    if (joker && card_counts[JOKER] > 0)
    {
        Card highest;
        for (map<Card, size_t>::iterator it = card_counts.begin(); it != card_counts.end(); ++it)
        {
            if (it->first != JOKER && it->second > card_counts[highest])
            {
                highest = it->first;
            }
        }
        int new_max = card_counts[highest] + card_counts[JOKER]; // number of cards in most plentiful non-joker
        switch (new_max)
        {
        case 5:
            score = 6;
            break;
        case 4:
            score = 5;
            break;
        case 3:
            card_counts[JOKER] == 2 ? score = 3 : score += 2;
            break;
        case 2:
            score = 1;
            break;
        }
    }
    return score;
}
