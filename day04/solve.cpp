#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <set>
#include <unordered_map>

using namespace std;

set<int> parse(string_view sv) {
    int pos = 0;
    set<int> result;
    string_view n;
    for (int i = 0; i < sv.size(); ++i) {
        if (sv[i] == ' ') {
            if (!n.empty()) result.emplace(stoi(n.data()));
            pos = i;
            n = string_view{};
            continue;
        }
        if (n.empty()) pos = i;
        n = sv.substr(pos, i - pos + 1);
    }
    if (!n.empty()) {
        result.emplace(stoi(n.data()));
    }
    return result;
}

int main(int argc, char **argv) {
    fstream file{argv[1]};
    string line;
    int points = 0;
    unordered_map<int, int> cards;
    int cardNo = 0;
    while (getline(file, line)) {
        ++cardNo;
        string_view vline{line};
        auto numbers = vline.substr(vline.find_first_of(':') + 1);
        auto winning_numbers = numbers.substr(0, numbers.find_first_of('|'));
        auto your_numbers = numbers.substr(numbers.find_first_of('|') + 1);
        auto winning_set = parse(winning_numbers);
        auto yours_set = parse(your_numbers);
        set<int> out;
        ranges::set_intersection(winning_set, yours_set,
                                 inserter(out, begin(out)));
        cards[cardNo] += 1;
        if (!out.empty()) {
            for (int i = cardNo + 1; i < cardNo + out.size() + 1; ++i) {
                cards[i] += cards[cardNo];
            }
            points += 1 << (out.size() - 1);
        }
    }
    cout << "Part 1: " << points << "\n";
    cout << "Part 2: "
         << accumulate(begin(cards), end(cards), 0,
                       [](int val, const auto &p) { return val + p.second; })
         << "\n";
}
