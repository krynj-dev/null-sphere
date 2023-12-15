#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

namespace day_12
{
    vector<pair<vector<long long>, string>> parse_records(vector<string>);
    pair<long long, vector<long long>> task_1(vector<pair<vector<long long>, string>>, map<pair<string, vector<long long>>, long long> &);
    long long task_2(vector<pair<vector<long long>, string>>, map<pair<string, vector<long long>>, long long> &);
} // namespace day_12

pair<int, int> valid_till(string &record, vector<long long> &sequence, size_t offset);
long long valid_combo(string &record, vector<long long> sequence, size_t offset, int to_fill, int remaining_wildcards,
                      map<pair<string, vector<long long>>, long long> &);

pair<long long, long long> aoc23::day_12()
{
    vector<string> lines = read_file("resources/input_12.txt", true);

    vector<pair<vector<long long>, string>> records = day_12::parse_records(lines);

    map<pair<string, vector<long long>>, long long> memo;

    pair<long long, vector<long long>> task_1_answer = day_12::task_1(records, memo);

    long long task_2_answer = day_12::task_2(records, memo);

    return {task_1_answer.first, task_2_answer};
}

pair<long long, vector<long long>> day_12::task_1(vector<pair<vector<long long>, string>> records, map<pair<string, vector<long long>>, long long> &memo)
{
    long long x = 0;

    vector<long long> valid_counts;
    for (auto kv : records)
    {
        int existing_hashes = count(kv.second.begin(), kv.second.end(), '#');
        int existing_wildcards = count(kv.second.begin(), kv.second.end(), '?');
        int to_fill = accumulate(kv.first.begin(), kv.first.end(), 0) - existing_hashes;
        long long c = valid_combo(kv.second, kv.first, 0, to_fill, existing_wildcards, memo);
        x += c;
    }

    return {x, valid_counts};
}

long long day_12::task_2(vector<pair<vector<long long>, string>> records, map<pair<string, vector<long long>>, long long> &memo)
{
    long long x = 0;
    // Unfold records once
    vector<pair<vector<long long>, string>> records_unfolded;
    for (size_t i = 0; i < records.size(); ++i)
    {
        vector<long long> seq = records[i].first;
        string comb = records[i].second;
        vector<long long> new_seq = records[i].first;
        string new_comb = records[i].second;
        for (size_t j = 1; j < 5; ++j)
        {
            new_seq.insert(new_seq.end(), seq.begin(), seq.end());
            new_comb += "?" + comb;
        }
        records_unfolded.push_back({new_seq, new_comb});
    }

    auto start = chrono::high_resolution_clock::now();
    int iter_count = 1;
    for (auto kv : records_unfolded)
    {
        int existing_hashes = count(kv.second.begin(), kv.second.end(), '#');
        int existing_wildcards = count(kv.second.begin(), kv.second.end(), '?');
        int to_fill = accumulate(kv.first.begin(), kv.first.end(), 0) - existing_hashes;
        long long c = valid_combo(kv.second, kv.first, 0, to_fill, existing_wildcards, memo);
        cout << "\r";
        auto tempdir = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start);
        cout << "Records processed: " << iter_count << "/" << records_unfolded.size() << "\t" << tempdir.count() << "s";
        iter_count++;
        x += c;
    }
    cout << "\r";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Time taken by function 2: " << duration.count() << " milliseconds" << std::endl;

    return x;
}

vector<pair<vector<long long>, string>> day_12::parse_records(vector<string> lines)
{
    vector<pair<vector<long long>, string>> records;
    for (auto line : lines)
    {
        vector<string> parts = split_string(line, string(1, ' '));
        records.push_back({parse_numbers(parts[1]), parts[0]});
    }
    return records;
}

// validates from offset onwards with the given sequence list. Returns -1 if invalid
pair<int, int> valid_till(string &record, vector<long long> &sequence, size_t offset)
{
    int hash_count = count(record.begin() + offset, record.end(), '#');
    int wc_count = count(record.begin() + offset, record.end(), '?');
    int total_hashes = accumulate(sequence.begin(), sequence.end(), 0);
    if (total_hashes > hash_count + wc_count)
    {
        return {-1, 0};
    }
    size_t cur_seq_loc = 0;
    size_t cur_seq_count = 0;
    size_t i;
    for (i = offset; i < record.size(); ++i)
    {
        char cur_char = record[i];
        if (cur_char == '.')
        {
            if (cur_seq_count == 0)
            {
                continue;
            }
            if (cur_seq_count != sequence[cur_seq_loc])
            {
                return {-1, 0};
            }
            cur_seq_loc++;
            cur_seq_count = 0;
        }
        else if (cur_char == '#')
        {
            cur_seq_count++;
        }
        else if (cur_char == '?')
        {
            return {max((int)(i - cur_seq_count) - 1, 0), cur_seq_loc};
        }
    }
    if (cur_seq_count > 0 && cur_seq_count != sequence[cur_seq_loc])
    {
        return {-1, 0};
    }
    return {i, cur_seq_loc};
}

long long valid_combo(string &record, vector<long long> sequence, size_t offset, int to_fill, int remaining_wildcards,
                      map<pair<string, vector<long long>>, long long> &memo)
{
    pair<string, vector<long long>> key = {record.substr(offset), sequence};
    if (memo.find(key) != memo.end())
    {
        return memo[key];
    }
    auto validated = valid_till(record, sequence, offset); // gives next wildcard or -1 if invalid
    if ((to_fill > remaining_wildcards) || validated.first == -1)
    {
        return 0;
    }
    if (validated.first == record.size() || (to_fill <= 0 && remaining_wildcards == 0))
    {
        return 1;
    }
    size_t wc_loc = record.find('?', validated.first);
    vector<string> combo_list;
    string hash_string = record;
    hash_string[wc_loc] = '#';
    string dot_string = record;
    dot_string[wc_loc] = '.';
    vector<long long> trimmed_sequence(sequence.begin() + validated.second, sequence.end());
    if (remaining_wildcards == to_fill)
    {
        // Don't recurse, just fill and send
        replace(hash_string.begin(), hash_string.end(), '?', '#');
        if (valid_till(hash_string, trimmed_sequence, validated.first).first == record.size())
        {
            return 1;
        }
        return 0;
    }
    else if (to_fill == 0 && remaining_wildcards > 0)
    {
        // Dont recurse, just fill and send
        replace(dot_string.begin(), dot_string.end(), '?', '.');
        auto v = valid_till(dot_string, trimmed_sequence, validated.first);
        if (v.first == record.size())
        {
            return 1;
        }
        return 0;
    }
    auto c_1 = valid_combo(hash_string, trimmed_sequence, validated.first, to_fill - 1, remaining_wildcards - 1, memo);
    auto c_2 = valid_combo(dot_string, trimmed_sequence, validated.first, to_fill, remaining_wildcards - 1, memo);
    memo[key] = c_1 + c_2;
    return c_1 + c_2;
}
