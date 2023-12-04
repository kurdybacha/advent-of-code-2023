#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

using namespace std;

unordered_set<int> parse(string_view sv) {
    unordered_set<int> result;
    for (const auto &n : sv | views::split(' ')) {
        result.emplace(stoi(n.data()));
    }
    return result;
}

unordered_set<int> intersection(const unordered_set<int> &a,
                                const unordered_set<int> &b) {
    auto view = a | views::filter([&b](int e) { return b.contains(e); });
    return unordered_set<int>(begin(view), end(view));
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
        auto numbers = vline.substr(vline.find_first_of(':') + 2);
        auto winning_set =
            parse(numbers.substr(0, numbers.find_first_of('|') - 1));
        auto my_set = parse(numbers.substr(numbers.find_first_of('|') + 2));
        auto my_winning = intersection(my_set, winning_set);
        cards[cardNo] += 1;
        if (!my_winning.empty()) {
            for (int i = cardNo + 1; i < cardNo + my_winning.size() + 1; ++i) {
                cards[i] += cards[cardNo];
            }
            points += 1 << (my_winning.size() - 1);
        }
    }
    cout << format("Part 1: {}\n", points);
    cout << format("Part 2: {}\n",
                   ranges::fold_left(cards | views::values, 0, plus<int>()));
}
