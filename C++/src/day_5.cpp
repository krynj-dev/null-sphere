#include <advent.h>
#include <util.h>

using namespace std;
using namespace aoc23;

class almanac
{
public:
    vector<long long> seeds;
    vector<std::tuple<long long, long long, long long>> seed_soil;
    vector<std::tuple<long long, long long, long long>> soil_fert;
    vector<std::tuple<long long, long long, long long>> fert_water;
    vector<std::tuple<long long, long long, long long>> water_light;
    vector<std::tuple<long long, long long, long long>> light_temp;
    vector<std::tuple<long long, long long, long long>> temp_hum;
    vector<std::tuple<long long, long long, long long>> hum_loc;
};

almanac parse_lines(vector<string>);
long long map_it(long long, vector<tuple<long long, long long, long long>>);
long long task_1(almanac);
long long task_2(almanac);
vector<pair<long long, long long>> split_ranges(pair<long long, long long>, vector<tuple<long long, long long, long long>>);
vector<tuple<long long, long long, long long>>::iterator get_range(long long, vector<tuple<long long, long long, long long>> &);
string tuple_string(tuple<long long, long long, long long>);
string pair_string(pair<long long, long long>);
vector<vector<tuple<long long, long long, long long>>> mapper_map(almanac);

pair<long long, long long> aoc23::day_5()
{
    vector<string> lines = read_file("resources/input_5.txt", true);

    almanac al = parse_lines(lines);

    return { task_1(al), task_2(al) };
}

long long task_1(almanac al)
{
    vector<long long> locs;
    for (long long s : al.seeds)
    {
        long long loc = map_it(map_it(map_it(map_it(map_it(map_it(map_it(s, al.seed_soil), al.soil_fert), al.fert_water), al.water_light), al.light_temp), al.temp_hum), al.hum_loc);
        locs.push_back(loc);
    }
    long long lowest = locs[0];
    for (long long loc : locs)
    {
        if (loc < lowest)
        {
            lowest = loc;
        }
    }

    return lowest;
}

almanac parse_lines(vector<string> lines)
{
    // Remove blank lines
    vector<string>::iterator it = lines.begin();
    while (it != lines.end())
    {
        if (it->empty())
        {
            it = lines.erase(it);
        }
        else
        {
            ++it;
        }
    }
    // Get seeds
    vector<long long> seeds = aoc23::parse_numbers(split_string(lines[0], ":")[1]);
    // Parse map
    almanac al = almanac();
    al.seeds = seeds;
    size_t parse_state = 0; // 0 - seed to soil; 1 - soil to fert; 2 - fert to water; 3 - water to light; 4 - light to temp; 5 - temp to hum; 6 - hum to loc
    for (long long i = 2; i < lines.size(); i++)
    {
        vector<long long> numbers = aoc23::parse_numbers(lines[i]);
        if (numbers.size() > 0)
        {
            switch (parse_state)
            {
            case 0:
                al.seed_soil.push_back(make_tuple(numbers[0], numbers[1], numbers[2]));
                break;
            case 1:
                al.soil_fert.push_back(make_tuple(numbers[0], numbers[1], numbers[2]));
                break;
            case 2:
                al.fert_water.push_back(make_tuple(numbers[0], numbers[1], numbers[2]));
                break;
            case 3:
                al.water_light.push_back(make_tuple(numbers[0], numbers[1], numbers[2]));
                break;
            case 4:
                al.light_temp.push_back(make_tuple(numbers[0], numbers[1], numbers[2]));
                break;
            case 5:
                al.temp_hum.push_back(make_tuple(numbers[0], numbers[1], numbers[2]));
                break;
            case 6:
                al.hum_loc.push_back(make_tuple(numbers[0], numbers[1], numbers[2]));
                break;
            default:
                break;
            }
        }
        else
        {
            parse_state += 1;
        }
    }

    return al;
}

long long map_it(long long x, vector<tuple<long long, long long, long long>> mapper)
{
    // Find the right range to use
    int matches = 0;
    long long y = x;
    for (int i = 0; i < mapper.size(); ++i)
    {
        tuple<long long, long long, long long> range_i = mapper[i];
        long long source_i = get<1>(range_i);
        long long dest_i = get<0>(range_i);
        long long length_i = get<2>(range_i);
        if (x >= source_i && x < source_i + length_i)
        {
            matches++;
            y = (x - source_i) + dest_i;
        }
    }
    if (matches > 1)
    {
        cout << "eek" << endl;
    }
    // cout << " ---> " << y;
    return y;
}

long long task_2(almanac al)
{
    // create seed range pairs
    vector<pair<long long, long long>> seed_pairs;
    for (size_t i = 0; i < al.seeds.size(); i += 2)
    {
        seed_pairs.push_back({al.seeds[i], al.seeds[i + 1]});
    }

    vector<long long> min_of_seeds;

    for (pair<long long, long long> seed : seed_pairs)
    {
        // cout << "STARTING SEED PAIR: " << seed.first << " " << seed.second << endl;
        vector<pair<long long, long long>> destinations;
        destinations.push_back(seed);
        for (vector<tuple<long long, long long, long long>> mapper : mapper_map(al))
        {
            // cout << "STARTING MAPPER" << endl;
            vector<pair<long long, long long>> nd;
            for (pair<long long, long long> rng : destinations)
            {
                vector<pair<long long, long long>> splt = split_ranges(rng, mapper);
                nd.insert(nd.end(), splt.begin(), splt.end());
            }
            destinations = nd;
        }

        long long lowest = destinations[0].first;
        for (pair<long long, long long> dest : destinations)
        {
            if (dest.first < lowest)
            {
                lowest = dest.first;
            }
        }
        min_of_seeds.push_back(lowest);
    }

    long long lowest = min_of_seeds[0];
    for (long long low : min_of_seeds)
    {
        if (low < lowest)
        {
            lowest = low;
        }
    }

    return lowest;
}

vector<pair<long long, long long>> split_ranges(pair<long long, long long> x, vector<tuple<long long, long long, long long>> mapper)
{
    vector<pair<long long, long long>> destinations;
    queue<pair<long long, long long>> ranges;
    ranges.push(x);
    while (!ranges.empty())
    {
        pair<long long, long long> pair_i = ranges.front();
        long long source_start = pair_i.first;
        long long source_range = pair_i.second;
        long long source_end = source_start + source_range - 1;
        ranges.pop();
        bool hit = false;
        for (tuple<long long, long long, long long> map : mapper)
        {
            long long map_start = get<1>(map);
            long long map_end = get<1>(map) + get<2>(map) - 1;
            long long offset = get<0>(map) - get<1>(map);
            // cout << "CASE: " << source_start << ", " << source_end << " : " << map_start << ", " << map_end << endl;
            if (source_start >= map_start && source_end <= map_end)
            {
                // cout << "DEST!: " << source_start + offset << ", " << source_range << endl;
                destinations.push_back({source_start + offset, source_range});
                hit = true;
                break;
            }
            if (source_end < map_start || source_start > map_end)
            {
                continue;
            }
            if (source_start < map_start)
            {
                hit = true;
                // cout << "Pushed a: " << source_start << ", " << (map_start - source_start) << endl;
                // cout << "Pushed: " << map_start << ", " << (source_end - map_start) << endl;
                ranges.push({source_start, (map_start - source_start)});
                ranges.push({map_start, source_end - map_start});
                break;
            }
            if (source_end > map_end)
            {
                hit = true;
                // cout << "Pushed b: " << source_start << ", " << (map_end - source_start) << endl;
                // cout << "Pushed: " << map_end << ", " << (source_end - map_end) << endl;
                ranges.push({source_start, (map_end - source_start)});
                ranges.push({map_end + 1, (source_end - map_end)});
                break;
            }
        }
        if (!hit)
        {
            // cout << "DEST!: " << pair_string(pair_i) << endl;
            destinations.push_back(pair_i);
        }
    }

    return destinations;
}

vector<tuple<long long, long long, long long>>::iterator get_range(long long x, vector<tuple<long long, long long, long long>> &mapper)
{
    // Find the right range to use
    vector<tuple<long long, long long, long long>>::iterator y = mapper.end();
    for (vector<tuple<long long, long long, long long>>::iterator it = mapper.begin(); it != mapper.end(); ++it)
    {
        tuple<long long, long long, long long> range_i = *it;
        long long source_i = get<1>(range_i);
        long long dest_i = get<0>(range_i);
        long long length_i = get<2>(range_i);
        if (x >= source_i && x < source_i + length_i)
        {
            return it;
        }
    }
    return y;
}

string tuple_string(tuple<long long, long long, long long> t)
{
    return "(" + to_string(get<0>(t)) + ", " + to_string(get<1>(t)) + ", " + to_string(get<2>(t)) + ")";
}

string pair_string(pair<long long, long long> p)
{
    return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";
}

vector<vector<tuple<long long, long long, long long>>> mapper_map(almanac al)
{
    vector<vector<tuple<long long, long long, long long>>> the_map(7);
    the_map[0] = al.seed_soil;
    the_map[1] = al.soil_fert;
    the_map[2] = al.fert_water;
    the_map[3] = al.water_light;
    the_map[4] = al.light_temp;
    the_map[5] = al.temp_hum;
    the_map[6] = al.hum_loc;
    return the_map;
}
