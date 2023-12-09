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
    auto limit = end(numbers);
    while (any_of(begin(numbers), limit, [](int v) { return v != 0; })) {
        for (auto it = begin(numbers); it != limit - 1; ++it) {
            *it = *(it + 1) - *it;
        }
        --limit;
    }
    return ranges::fold_left(numbers, 0, plus<int>{});
}

int main(int argc, char** argv) {
    fstream file{argv[1]};
    string line;

    int sum = 0;
    int sum_reverse = 0;
    while (getline(file, line)) {
        auto numbers = parse(line);
        sum += predict(numbers);
        reverse(begin(numbers), end(numbers));
        sum_reverse += predict(numbers);
    }
    cout << format("Part 1: {}\n", sum);
    cout << format("Part 2: {}\n", sum_reverse);
}
