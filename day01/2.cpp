#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <string_view>
#include <vector>

using namespace std;

optional<int> digit_from_string(string_view str) {
    static vector<string_view> digit_strs = {
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    if (str.empty()) return {};

    if (isdigit(str.front())) return str.front() - '0';

    for (auto it = begin(digit_strs); it != end(digit_strs); ++it) {
        if (str.starts_with(*it)) {
            return std::distance(begin(digit_strs), it) + 1;
        }
    }

    return {};
}

int main(int, char **) {
    fstream f{"input.txt"};
    string line;
    uint64_t sum = 0;
    while (getline(f, line)) {
        for (auto it = begin(line); it != end(line); ++it) {
            auto digit = digit_from_string(string_view{it, end(line)});
            if (digit) {
                sum += *digit * 10;
                break;
            }
        }
        for (auto it = rbegin(line); it != rend(line); ++it) {
            auto digit =
                digit_from_string(string_view{it.base() - 1, end(line)});
            if (digit) {
                sum += *digit;
                break;
            }
        }
    }
    std::cout << sum << std::endl;
    return 0;
}
