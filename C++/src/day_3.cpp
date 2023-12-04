#include <advent.h>
#include <util.h>
#include <iostream>
#include <cassert>
#include <regex>

struct PartNumber
{
    PartNumber(size_t pos, size_t len)
    {
        this->pos = pos;
        this->len = len;
    }

public:
    size_t pos;
    size_t len;
};


vector<PartNumber> parse_schematic(std::vector<std::string> lines);
pair<size_t, size_t> pos_to_coords(size_t pos, size_t width);
size_t coords_to_pos(pair<int, int> coords, size_t width);
size_t token_type(char token);
void eval_symbol(pair<size_t, size_t> coords, std::vector<std::string> lines, vector<vector<size_t>> &aux,
                 size_t column_length, size_t row_width, size_t last_type);

void aoc23::day_3()
{
    std::vector<std::string> lines = read_file("resources/input_3.txt");
    // Strip newline
    for (size_t i = 0; i < lines.size(); i++)
    {
        lines[i] = lines[i].erase(lines[i].find_last_not_of("\r\n") + 1, lines[i].size());
    }

    size_t line_len = lines[0].size();
    for (vector<string>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        assert(it->size() == line_len);
    }

    vector<PartNumber> part_numbers = parse_schematic(lines);
    // Task 1
    int total = 0;
    for (PartNumber pn: part_numbers)
    {
        pair<size_t, size_t> coords = pos_to_coords(pn.pos, lines[0].size());
        total += stoi(lines[coords.second].substr(coords.first, pn.len));
    }
    cout << "\tTask 1: total is " << total << endl;
}

vector<PartNumber> parse_schematic(std::vector<std::string> lines)
{
    vector<PartNumber> part_numbers;

    size_t row_width = lines[0].size();
    size_t column_length = lines.size();

    vector<vector<size_t>> aux(column_length);
    for (size_t y = 0; y < aux.size(); y++)
    {
        aux[y] = vector<size_t>(row_width);
    }

    size_t pos = 0;
    size_t last_type = 0;
    size_t num_len = 0;
    bool is_part = false;

    // Main loop
    while (pos < row_width * column_length)
    {
        pair<size_t, size_t> coords = pos_to_coords(pos, row_width);
        char token = lines[coords.second][coords.first];
        size_t cur_type = token_type(token);
        // Skip if .
        if (cur_type == 0)
        {
            if (last_type == 1)
            {
            }
            last_type = 0;
        }
        // If token is digit 0-9
        else if (cur_type == 1)
        {
            num_len += 1;
            if (last_type == 2 || (last_type == 1 && is_part) || aux[coords.second][coords.first] == 1)
            {
                is_part = true;
            }
            else if (last_type == 1 && !is_part)
            {
                if (((coords.second != column_length - 1) && (coords.first != row_width - 1)) && (token_type(lines[coords.second + 1][coords.first + 1]) == 2))
                {
                    if (coords.first < row_width - 3)
                    {
                        aux[coords.second][coords.first + 2] = 1;
                    }
                    is_part = true;
                }
            }
            else // New number, not previously marked as good. Most checking
            {
                if (coords.second != column_length - 1)
                {
                    if (coords.first != 0) // SW
                    {
                        pair<size_t, size_t> symbol_coords(coords.first - 1, coords.second + 1);
                        if (token_type(lines[symbol_coords.second][symbol_coords.first]))
                        {
                            eval_symbol(symbol_coords, lines, aux, column_length, row_width, last_type);
                            is_part = true;
                        }
                    }
                    // S
                    pair<size_t, size_t> symbol_coords(coords.first, coords.second + 1);
                    if (token_type(lines[symbol_coords.second][symbol_coords.first]) == 2)
                    {
                        eval_symbol(symbol_coords, lines, aux, column_length, row_width, last_type);
                        is_part = true;
                    }
                    if (coords.first != row_width - 1) // SE
                    {
                        pair<size_t, size_t> symbol_coords(coords.first + 1, coords.second + 1);
                        if (token_type(lines[symbol_coords.second][symbol_coords.first]))
                        {
                            aux[coords.second][coords.first + 2] = 1;
                            eval_symbol(symbol_coords, lines, aux, column_length, row_width, last_type);
                            is_part = true;
                        }
                    }
                }
            }
            last_type = 1;
        }
        // If token is symbol
        else
        {
            if (last_type == 1)
            {
                is_part = true;
            }
            // Skip if already evaluated
            if (aux[coords.second][coords.first] < 2)
            {
                eval_symbol(coords, lines, aux, column_length, row_width, last_type);
            }
            last_type = 2;
        }

        if (cur_type == 0 || cur_type == 2 || (coords.first == row_width - 1 && cur_type == 1))
        {
            int length_mod = (coords.first == row_width - 1 && cur_type == 1) ? 1 : 0;
            if (is_part)
            {
                part_numbers.push_back(PartNumber(pos - num_len + length_mod, num_len));
            }
            // Reset number and token trackers
            num_len = 0;
            is_part = false;
        }

        pos++;
    }

    return part_numbers;
}

pair<size_t, size_t> pos_to_coords(size_t pos, size_t width)
{
    return make_pair(pos % width, pos / width);
}

size_t coords_to_pos(pair<int, int> coords, size_t width)
{
    return coords.second * width + coords.first;
}

size_t token_type(char token)
{
    std::regex digit_regex("[0-9]");

    if (token == '.')
        return 0;

    if (std::regex_match(string(1, token), digit_regex))
        return 1;

    return 2;
}

void eval_symbol(pair<size_t, size_t> coords, std::vector<std::string> lines, vector<vector<size_t>> &aux,
                 size_t column_length, size_t row_width, size_t last_type)
{
    // Set aux below and next to 1
    if ((coords.second != column_length - 1) && (last_type != 2))
    {
        if (coords.first != 0)
        {
            aux[coords.second + 1][coords.first - 1] = 1;
        }
        aux[coords.second + 1][coords.first] = 1;
    }
    if (coords.first != row_width - 1)
    {
        aux[coords.second][coords.first + 1] = 1;
    }
    if ((coords.second != column_length - 1) && (coords.first != row_width - 1))
    {
        aux[coords.second + 1][coords.first + 1] = 1;
    }
    aux[coords.second][coords.first] = 2;
}