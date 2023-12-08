#include <assert.h>
#include <bits/ranges_cmp.h>

#include <algorithm>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    fstream file{argv[1]};
    string line;

    getline(file, line);
    string directions = line;
    // skip empty line
    getline(file, line);

    unordered_map<string, pair<string, string>> nodes;

    vector<string> poss;
    while (getline(file, line)) {
        string node = line.substr(0, 3);
        string left = line.substr(7, 3);
        string right = line.substr(12, 3);
        nodes.emplace(node, make_pair(left, right));
        if (node.back() == 'A') {
            poss.push_back(node);
        }
    }

    vector<ulong> counts(poss.size());
    for (const auto& [idx, pos] : poss | views::enumerate) {
        ulong i = 0;
        string p = pos;
        while (p.back() != 'Z') {
            char direction = directions[i % directions.size()];
            auto d = nodes[p];
            auto next = direction == 'L' ? d.first : d.second;
            p = next;
            ++i;
        }
        counts[idx] = i;
    }
    cout << format(
        "Part 1: {}\n",
        counts[distance(begin(poss), find(begin(poss), end(poss), "AAA"))]);

    auto l = ranges::fold_left(counts, 1,
                               [](ulong a, ulong b) { return lcm(a, b); });

    cout << format("Part 2: {}\n", l);
}
