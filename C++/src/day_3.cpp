#include <advent.h>
#include <util.h>

size_t task_one_filter(const string &line, const char *, const size_t &offset);
size_t task_two_filter(const string &line, const char *, const size_t &offset);
vector<string> parse_schematic(std::vector<std::string> &lines, char *,
                               size_t (*find_next_symbol)(const string &, const char *, const size_t &),
                               bool ignore_duplicates, map<pair<size_t, size_t>, vector<string>> &symbol_part_map);
void eval_symbol(const size_t x, const size_t y, const size_t row_width, const size_t column_length, 
    std::vector<std::string> lines, vector<string> &part_numbers, vector<vector<size_t>> &aux, bool ignore_duplicates,
    map<pair<size_t, size_t>, vector<string>> &symbol_part_map);

void aoc23::day_3()
{
    std::vector<std::string> lines = read_file("resources/input_3.txt", true);
    // Strip newline
    for (size_t i = 0; i < lines.size(); i++)
    {
        lines[i] = lines[i].erase(lines[i].find_last_not_of("\r\n") + 1, lines[i].size());
    }

    char *non_symbols = "0123456789.";
    map<pair<size_t, size_t>, vector<string>> symbol_part_map_t1;
    vector<string> part_numbers = parse_schematic(lines, non_symbols, &task_one_filter, true, symbol_part_map_t1);

    // Get sum
    int total = 0;
    for (string s: part_numbers)
    {
        total += stoi(s);
    }
    cout << "\tTask 1: total is " << total << endl;

    char* only_asterisk = "*";
    map<pair<size_t, size_t>, vector<string>> symbol_part_map_t2;
    vector<string> t2 = parse_schematic(lines, only_asterisk, &task_two_filter, false, symbol_part_map_t2);
    int total2 = 0;
    for (const auto& pair : symbol_part_map_t2) {
        if (pair.second.size() == 2)
        {
            total2 += stoi(pair.second[0]) * stoi(pair.second[1]);
        }
    }
    cout << "\tTask 2: total is " << total2 << endl;
}

vector<string> parse_schematic(std::vector<std::string> &lines, char *symbol_set,
                               size_t (*find_next_symbol)(const string &, const char *, const size_t &),
                               bool ignore_duplicates, map<pair<size_t, size_t>, vector<string>> &symbol_part_map)
{
    vector<string> part_numbers;

    size_t x;
    size_t row_width = lines[0].size();
    size_t column_length = lines.size();

    vector<vector<size_t>> aux(lines.size());
    for (size_t y = 0; y < aux.size(); y++)
    {
        aux[y] = vector<size_t>(row_width);
    }

    for (size_t y = 0; y < column_length; y++)
    {
        std::string line = lines[y];
        x = find_next_symbol(line, symbol_set, 0);
        for (x; x != string::npos; x = find_next_symbol(line, symbol_set, x + 1))
        {
            eval_symbol(x, y, row_width, column_length, lines, part_numbers, aux, ignore_duplicates, symbol_part_map);
        }
    }

    return part_numbers;
}

size_t task_one_filter(const string &line, const char *symbol_set, const size_t &offset)
{
    return line.find_first_not_of(symbol_set, offset);
}

size_t task_two_filter(const string &line, const char *symbol_set, const size_t &offset)
{
    return line.find_first_of(symbol_set, offset);
}

void eval_symbol(const size_t x, const size_t y, const size_t row_width, const size_t column_length, 
    std::vector<std::string> lines, vector<string> &part_numbers, vector<vector<size_t>> &aux, bool ignore_duplicates,
    map<pair<size_t, size_t>, vector<string>> &symbol_part_map)
{
    size_t pos_x = max(0, int(x) - 1);
    size_t pos_y = max(0, int(y) - 1);
    size_t max_x = min(int(x) + 1, int(row_width) - 1);
    size_t max_y = min(int(y) + 1, int(row_width) - 1);
    string digits = "0123456789";

    while (pos_y <= max_y)
    {
        while (pos_x <= max_x)
        {
            if (aux[pos_y][pos_x] == 0 || !ignore_duplicates)
            {
                string line = lines[pos_y];
                if (digits.find(line[pos_x]) != string::npos) // is digit
                {
                    size_t i_start = pos_x;
                    while (i_start - 1 >= 0 && digits.find(line[i_start-1]) != string::npos)
                    {
                        i_start--;
                        aux[pos_y][i_start] = 1;
                    }
                    size_t i_end = pos_x;
                    while (i_end + 1 < line.size() && digits.find(line[i_end+1]) != string::npos)
                    {
                        i_end++;
                        aux[pos_y][i_end] = 1;
                    }
                    part_numbers.push_back(line.substr(i_start, i_end - i_start + 1));
                    symbol_part_map[make_pair(x, y)].push_back(line.substr(i_start, i_end - i_start + 1));
                    pos_x = i_end;
                }
            }
            pos_x++;
        }
        pos_x = max(0, int(x) - 1);
        pos_y++;
    }
}
