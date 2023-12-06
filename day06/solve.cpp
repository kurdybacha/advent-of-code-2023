#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

using namespace std;

vector<uint64_t> parse(string_view sv) {
    vector<uint64_t> result;
    for (const auto &n : sv | views::split(' ')) {
        if (n.empty()) continue;
        result.emplace_back(stol(n.data()));
    }
    return result;
}

uint64_t concat(const vector<uint64_t> nums) {
    string str;
    for (auto n : nums) {
        str += to_string(n);
    }
    return stol(str);
}

uint64_t count_new_records(uint64_t record, uint64_t time) {
    uint64_t count = 0;
    for (uint64_t i = 0; i <= time; ++i) {
        uint64_t dist = i * (time - i);
        if (dist > record) {
            count += 1;
        }
    }
    return count;
}

int main(int argc, char **argv) {
    fstream file{argv[1]};
    string line;
    getline(file, line);
    string_view sline(line);
    auto times = parse(sline.substr(sline.find_first_of(':') + 1));
    getline(file, line);
    sline = string_view{line};
    auto records = parse(sline.substr(sline.find_first_of(':') + 1));

    vector<uint64_t> counts(times.size());
    for (const auto &[idx, time] : views::enumerate(times)) {
        counts[idx] += count_new_records(records[idx], time);
    }

    cout << format("Part 1: {}\n", accumulate(begin(counts), end(counts), 1,
                                              multiplies<uint64_t>()));

    uint64_t time = concat(times);
    uint64_t record = concat(records);
    cout << format("Part 2: {}\n", count_new_records(record, time));
}
