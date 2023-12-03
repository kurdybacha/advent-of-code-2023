#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <unordered_map>

using namespace std;

int main(int argc, char **argv) {
    fstream file(argv[1]);
    string line;
    int product_sum = 0;
    while (getline(file, line)) {
        string_view sv{line};
        string_view sets = sv.substr(sv.find_first_of(':') + 1);
        unordered_map<string_view, int> colors = {
            {"red", 0},
            {"green", 0},
            {"blue", 0},
        };
        for (const auto &set : ranges::split_view(sets, ';')) {
            for (const auto &c : ranges::split_view(set, ',')) {
                string_view cubes{c.begin(), c.end()};
                int count =
                    stoi(cubes.substr(1, cubes.find_last_of(' ')).data());
                string_view color = cubes.substr(cubes.find_last_of(' ') + 1);
                if (colors[color] < count) {
                    colors[color] = count;
                }
            }
        }
        product_sum += ranges::fold_left(
            colors, 1,
            [](int product, const auto &p) { return product * p.second; });
    }
    cout << product_sum << "\n";
    return 0;
}
