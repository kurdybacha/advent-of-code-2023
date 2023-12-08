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

int main(int argc, char **argv) {
    fstream file{argv[1]};
    string line;

    getline(file, line);
    string directions = line;
    // skip empty line
    getline(file, line);

    unordered_map<string, pair<string, string>> nodes;

    while (getline(file, line)) {
        string node = line.substr(0, 3);
        string left = line.substr(7, 3);
        string right = line.substr(12, 3);
        nodes.emplace(node, make_pair(left, right));
    }

    string pos = "AAA";
    int i = 0;
    while (pos != "ZZZ") {
        auto &d = nodes[pos];
        auto &next =
            directions[i % directions.size()] == 'L' ? d.first : d.second;
        pos = next;
        i++;
    }
    cout << format("Part 1: {}\n", i);
}
