#include <algorithm>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <ranges>
#include <vector>

using namespace std;

struct Range {
    uint64_t first = 0;
    uint64_t last = 0;
    uint64_t dest = 0;
    bool contains(Range r) const {
        return (r.first >= first && r.first < last) ||
               (r.last <= last && r.last > first);
    }
    uint64_t size() const { return last - first; }
    uint64_t empty() const { return size() == 0; }
    Range intersection(Range r) const {
        if (contains(r)) {
            uint64_t f = r.first >= first && r.first < last ? r.first : first;
            uint64_t l = r.last <= last && r.last > first ? r.last : last;
            return Range{f, l, dest};
        }
        return Range{};
    }

    Range diff(Range r) const {
        Range i = intersection(r);
        if (!i.empty()) {
            if (r.first < i.first) {
                return Range{r.first, i.first};
            } else if (r.last > i.last) {
                return Range{i.last, r.last};
            }
            return i;
        }
        return Range{};
    }

    Range map(Range r) const {
        Range inter = intersection(r);
        if (!inter.empty()) {
            return Range{dest + (inter.first - first),
                         dest + (inter.last - first)};
        }
        return r;
    }
};

using RangeSet = vector<Range>;

struct Mapper {
    RangeSet ranges;

    RangeSet map(RangeSet range_set) const {
        RangeSet result;
        list<Range> range_list;
        for (auto &&r : range_set) {
            range_list.emplace_back(r);
        }
        while (!range_list.empty()) {
            bool include = true;
            for (const auto &range : ranges) {
                auto &first = range_list.front();
                auto inter = range.intersection(first);
                if (inter.empty()) {
                    continue;
                } else {
                    include = false;
                    result.push_back(range.map(inter));
                    if (inter.size() != first.size()) {
                        Range d = range.diff(first);
                        range_list.push_back(d);
                    }
                    break;
                }
            }

            if (include) {
                result.push_back(range_list.front());
            }

            range_list.pop_front();
        }
        return result;
    }
};

using MapperSet = vector<Mapper>;

struct Almanac {
    MapperSet mappers;

    RangeSet map(RangeSet range_set) const {
        for (const auto &mapper : mappers) {
            range_set = mapper.map(range_set);
        }
        return range_set;
    }
};

vector<uint64_t> parse(string_view sv) {
    vector<uint64_t> result;
    for (const auto &n : sv | views::split(' ')) {
        result.emplace_back(stol(n.data()));
    }
    return result;
}

int main(int argc, char **argv) {
    fstream file{argv[1]};
    string line;
    getline(file, line);
    string_view vline{line};

    auto seeds = parse(vline.substr(vline.find_first_of(':') + 2));

    RangeSet seed_ranges[2];

    RangeSet seeds_ranges_1;
    for (int i = 0; i < seeds.size(); ++i) {
        seed_ranges[0].emplace_back(Range{seeds[i], seeds[i] + 1, seeds[i]});
    }

    for (int i = 0; i < seeds.size(); i += 2) {
        seed_ranges[1].emplace_back(
            Range{seeds[i], seeds[i] + seeds[i + 1], 0});
    }

    Almanac almanac;

    while (getline(file, line)) {
        if (line.empty()) continue;
        string_view vline{line};
        if (!isdigit(vline.at(0))) {
            almanac.mappers.emplace_back(Mapper{});
        } else {
            auto v = parse(vline);
            almanac.mappers.back().ranges.emplace_back(
                Range{v[1], v[1] + v[2], v[0]});
        }
    }

    for (const auto &[idx, sr] : views::enumerate(seed_ranges)) {
        RangeSet mapped_ranges = almanac.map(sr);
        auto it = min_element(
            begin(mapped_ranges), end(mapped_ranges),
            [](const Range &a, const Range &b) { return a.first < b.first; });
        cout << format("Part {}: {}\n", idx + 1, it->first);
    }
}
