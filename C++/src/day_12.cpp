#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

namespace day_12
{
    vector<pair<vector<long long>, string>> parse_records(vector<string>);
    pair<long long, vector<long long>> task_1(vector<pair<vector<long long>, string>>);
    long long task_2(vector<pair<vector<long long>, string>>, vector<long long>);
} // namespace day_12

pair<int, int> valid_till(string record, vector<long long> sequence, size_t offset);
vector<string> valid_combo(string record, vector<long long> sequence, size_t offset, int to_fill, int remaining_wildcards);

pair<long long, long long> aoc23::day_12()
{
    vector<string> lines = read_file("resources/input_12.txt", true);

    vector<pair<vector<long long>, string>> records = day_12::parse_records(lines);

    // for (auto kv : records)
    // {
    //     cout << "Numbers: ";
    //     for (auto n : kv.first)
    //     {
    //         cout << n << " ";
    //     }
    //     cout << kv.second << endl;
    // }

    pair<long long, vector<long long>> task_1_answer = day_12::task_1(records);

    cout << "T1 done: " << task_1_answer.first << endl;

    long long task_2_answer = day_12::task_2(records, task_1_answer.second);

    return {task_1_answer.first, task_2_answer};
}

pair<long long, vector<long long>> day_12::task_1(vector<pair<vector<long long>, string>> records)
{
    long long x = 0;

    auto start = chrono::high_resolution_clock::now();
    vector<long long> valid_counts;
    for (auto kv : records)
    {
        int existing_hashes = count(kv.second.begin(), kv.second.end(), '#');
        int existing_wildcards = count(kv.second.begin(), kv.second.end(), '?');
        int to_fill = accumulate(kv.first.begin(), kv.first.end(), 0) - existing_hashes;
        vector<string> c = valid_combo(kv.second, kv.first, 0, to_fill, existing_wildcards);
        valid_counts.push_back(c.size());
        x += c.size();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken by function 1: " << duration.count() << " milliseconds" << std::endl;

    return {x, valid_counts};
}

long long day_12::task_2(vector<pair<vector<long long>, string>> records, vector<long long> task_1_counts)
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

    map<pair<string, vector<long long>>, long long> memo;

    auto start = chrono::high_resolution_clock::now();
    int iter_count = 1;
    for (auto kv : records_unfolded)
    {
        // cout << kv.second << endl;
        int existing_hashes = count(kv.second.begin(), kv.second.end(), '#');
        int existing_wildcards = count(kv.second.begin(), kv.second.end(), '?');
        int to_fill = accumulate(kv.first.begin(), kv.first.end(), 0) - existing_hashes;
        vector<string> c = valid_combo(kv.second, kv.first, 0, to_fill, existing_wildcards);
        cout << "\r";
        auto tempdir = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start);
        cout << "Records processed: " << iter_count << "/" << records_unfolded.size() << "\t" << tempdir.count() << "s";
        iter_count++;
        x += c.size();
    }
    cout << endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken by function 2: " << duration.count() << " milliseconds" << std::endl;

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
pair<int, int> valid_till(string record, vector<long long> sequence, size_t offset)
{
    int hash_count = count(record.begin() + offset, record.end(), '#');
    int wc_count = count(record.begin() + offset, record.end(), '?');
    int total_hashes = accumulate(sequence.begin(), sequence.end(), 0);
    if (total_hashes > hash_count + wc_count)
    {
        // cout << "not possibly valid (a): " << record << endl;
        return {-1, 0};
    }
    size_t cur_seq_loc = 0;
    size_t cur_seq_count = 0;
    size_t i;
    for (i = offset; i < record.size(); ++i)
    {
        char cur_char = record[i];
        // cout << "DEBUG: " << i << " " << cur_char << " " << cur_seq_loc << " " << cur_seq_count << " " << sequence[cur_seq_loc] <<   endl;
        if (cur_char == '.')
        {
            if (cur_seq_count == 0)
            {
                continue;
            }
            if (cur_seq_count != sequence[cur_seq_loc])
            {
                // cout << "not possibly valid (b): " << record << endl;
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
            // cout << "valid: " << record << " till " << max((int)(i - cur_seq_count) - 1, 0) << ", successfully got through " << cur_seq_loc << " sequence parts" << endl;
            return {max((int)(i - cur_seq_count) - 1, 0), cur_seq_loc};
        }
    }
    // final check
    if (cur_seq_count > 0 && cur_seq_count != sequence[cur_seq_loc])
    {
        // cout << "not possibly valid (b): " << record << endl;
        return {-1, 0};
    }
    // cout << "fully valid? " << (i == record.size()) << endl;
    return {i, cur_seq_loc};
}

vector<string> valid_combo(string record, vector<long long> sequence, size_t offset, int to_fill, int remaining_wildcards)
{
    auto validated = valid_till(record, sequence, offset); // gives next wildcard or -1 if invalid
    if ((to_fill > remaining_wildcards) || validated.first == -1)
    {
        return {};
    }
    if (validated.first == record.size() || (to_fill <= 0 && remaining_wildcards == 0))
    {
        // cout << "found valid (a): " << record << endl;
        return {record};
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
            // cout << "found valid (b): " << hash_string << endl;
            return {hash_string};
        }
        return {};
    }
    else if (to_fill == 0 && remaining_wildcards > 0)
    {
        // Dont recurse, just fill and send
        replace(dot_string.begin(), dot_string.end(), '?', '.');
        auto v = valid_till(dot_string, trimmed_sequence, validated.first);
        if (v.first == record.size())
        {
            // cout << "found valid (c): " << dot_string << " with " << validated.first << " and " << validated.second << endl;
            return {dot_string};
        }
        return {};
    }
    else
    {
        auto c_1 = valid_combo(hash_string, trimmed_sequence, validated.first, to_fill - 1, remaining_wildcards - 1);
        auto c_2 = valid_combo(dot_string, trimmed_sequence, validated.first, to_fill, remaining_wildcards - 1);
        if (c_1.size() > 0)
        {
            combo_list.insert(combo_list.end(), c_1.begin(), c_1.end());
        }
        if (c_2.size() > 0)
        {
            combo_list.insert(combo_list.end(), c_2.begin(), c_2.end());
        }
    }
    return combo_list;
}
