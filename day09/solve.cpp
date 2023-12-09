#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

using namespace std;

vector<int> parse(const string_view sv) {
    vector<int> r;
    for (auto w : sv | views::split(' ')) {
        r.emplace_back(stoi(w.data()));
    }
    return r;
}

int predict(vector<int> numbers) {
    auto stop = end(numbers);
    while (!all_of(begin(numbers), stop, [](int v) { return v == 0; })) {
        for (auto it = begin(numbers); it != stop - 1; ++it) {
            *it = *(it + 1) - *it;
        }
        --stop;
    }
    return ranges::fold_left(numbers, 0, plus<int>());
}

int main(int argc, char** argv) {
    fstream file{argv[1]};
    string line;

    int sum = 0;
    while (getline(file, line)) {
        sum += predict(parse(line));
    }
    cout << format("Part 1: {}\n", sum);
}
