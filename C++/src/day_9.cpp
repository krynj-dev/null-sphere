#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

long long task_1(vector<vector<vector<long long>>>);
long long task_2(vector<vector<vector<long long>>>);

vector<vector<long long>> parse_sequences(vector<string>);
vector<vector<long long>> differential_sequences(vector<long long>);
long long extrapolate_from_history(vector<vector<long long>>);
long long extrapolate_from_history_backwards(vector<vector<long long>>);

pair<long long, long long> aoc23::day_9()
{
    vector<string> lines = read_file("resources/input_9.txt", true);

    vector<vector<long long>> sequences = parse_sequences(lines);

    vector<vector<vector<long long>>> histories;
    for (auto seq : sequences)
    {
        vector<vector<long long>> hist = differential_sequences(seq);
        histories.push_back(hist);
    }

    long long task_1_answer = task_1(histories);

    long long task_2_answer = task_2(histories);

    return {task_1_answer, task_2_answer};
}

long long task_1(vector<vector<vector<long long>>> histories)
{
    return accumulate(histories.begin(), histories.end(), 0LL,
                      [](long long acc, auto x)
                      {
                          return acc + extrapolate_from_history(x);
                      });
}

long long task_2(vector<vector<vector<long long>>> histories)
{
    return accumulate(histories.begin(), histories.end(), 0LL,
                      [](long long acc, auto x)
                      {
                          return acc + extrapolate_from_history_backwards(x);
                      });
}

vector<vector<long long>> parse_sequences(vector<string> lines)
{
    vector<vector<long long>> seqs;
    for (string line : lines)
    {
        if (!line.empty())
        {
            seqs.push_back(parse_numbers(line));
        }
    }
    return seqs;
}

vector<vector<long long>> differential_sequences(vector<long long> base_sequence)
{
    vector<vector<long long>> dif_seqs;
    dif_seqs.push_back(base_sequence);
    vector<long long> cur_seq = base_sequence;
    while (accumulate(cur_seq.begin(), cur_seq.end(), 0LL,
                      [](long long acc, long long x)
                      {
                          return acc + abs(x);
                      }) != 0)
    {
        vector<long long> new_seq;
        for (size_t i = 0; i < cur_seq.size() - 1; ++i)
        {
            new_seq.push_back(cur_seq[i + 1] - cur_seq[i]);
        }
        dif_seqs.push_back(new_seq);
        cur_seq = new_seq;
    }
    return dif_seqs;
}

long long extrapolate_from_history(vector<vector<long long>> history)
{
    long long a = 0;
    for (auto seq : history)
    {
        long long add_it = (seq.size() > 0 ? seq.back() : 0LL);
        a += add_it;
    }
    return a;
}

long long extrapolate_from_history_backwards(vector<vector<long long>> history)
{
    long long a = 0;
    for (int i = history.size() - 2; i >= 0; --i)
    {
        auto seq = history[i];
        
        a = seq.front() - a;
    }
    return a;
}
