#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

bool issymbol(char c) { return c != '.' && !isdigit(c); }

int harvest_sum(list<pair<int, string>> &nums, vector<int> &syms) {
    int sum = 0;
    auto it = begin(nums);
    while (it != end(nums)) {
        auto &[idx, num_str] = *it;
        auto sit = find_if(begin(syms), end(syms), [&](int s) {
            return s >= (idx > 0 ? idx - 1 : idx) &&
                   s <= idx + num_str.length();
        });
        if (sit != end(syms)) {
            sum += stoi(num_str.data());
            nums.erase(it++);
        } else {
            ++it;
        }
    }
    return sum;
}

int main(int argc, char **argv) {
    fstream file{argv[1]};
    string line;

    // coordinates of symbols in the last two rows.
    array<vector<int>, 2> syms;
    // numbers in the last two rows.
    array<list<pair<int, string>>, 2> nums;

    int sum = 0;
    int idx = 0;
    while (getline(file, line)) {
        string_view l{line};
        string_view number;
        int num_idx = 0;
        for (int i = 0; i < line.size(); ++i) {
            if (!isdigit(line.at(i))) {
                if (!number.empty())
                    nums[idx].push_back(
                        {num_idx, {begin(number), end(number)}});
                number = string_view();
                num_idx = i;
            }
            if (issymbol(line.at(i))) {
                syms[idx].push_back(i);
            }
            if (isdigit(line.at(i))) {
                if (number.empty()) num_idx = i;
                number = l.substr(num_idx, i - num_idx + 1);
            }
        }
        if (!number.empty())
            nums[idx].push_back({num_idx, {begin(number), end(number)}});

        sum += harvest_sum(nums[idx], syms[idx]);
        sum += harvest_sum(nums[idx], syms[abs(idx - 1) % syms.size()]);
        sum += harvest_sum(nums[abs(idx - 1) % nums.size()], syms[idx]);
        nums[abs(idx - 1) % nums.size()].clear();
        syms[abs(idx - 1) % syms.size()].clear();
        idx = (idx + 1) % syms.size();
    }

    cout << sum << "\n";
}
