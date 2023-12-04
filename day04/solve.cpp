#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <set>

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
    while (getline(file, line)) {
        string_view vline{line};
        auto numbers = vline.substr(vline.find_first_of(':') + 1);
        auto winning_numbers = numbers.substr(0, numbers.find_first_of('|'));
        auto your_numbers = numbers.substr(numbers.find_first_of('|') + 1);
        auto winning_set = parse(winning_numbers);
        auto yours_set = parse(your_numbers);
        set<int> out;
        ranges::set_intersection(winning_set, yours_set,
                                 inserter(out, begin(out)));
        if (!out.empty()) {
            points += 1 << (out.size() - 1);
        }
    }
    cout << points << "\n";
}
