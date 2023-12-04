#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>

using namespace std;

bool issymbol(char c) { return c != '.' && !isdigit(c); }

struct Sym {
    Sym(char s, int idx) : s{s}, idx{idx} {}
    char s;
    int idx;
    vector<int> adj_nums;
};

struct Num {
    Num(int idx, string str) : idx{idx}, str{str} {}
    int idx;
    string str;
    int val() const { return stoi(str); }
};

void find_adj_nums(list<Num> &nums, vector<Sym> &syms) {
    auto it = begin(nums);
    while (it != end(nums)) {
        auto &num = *it;
        auto sit = find_if(begin(syms), end(syms), [&](Sym sym) {
            return sym.idx >= (num.idx > 0 ? num.idx - 1 : num.idx) &&
                   sym.idx <= num.idx + num.str.length();
        });
        if (sit != end(syms)) {
            sit->adj_nums.push_back(num.val());
            nums.erase(it++);
        } else {
            ++it;
        }
    }
}

int main(int argc, char **argv) {
    fstream file{argv[1]};
    string line;

    // coordinates of symbols in the last two rows.
    array<vector<Sym>, 2> syms;
    // numbers in the last two rows.
    array<list<Num>, 2> nums;

    int sum = 0;
    int pow = 0;
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
                syms[idx].push_back({line.at(i), i});
            }
            if (isdigit(line.at(i))) {
                if (number.empty()) num_idx = i;
                number = l.substr(num_idx, i - num_idx + 1);
            }
        }
        if (!number.empty())
            nums[idx].push_back({num_idx, {begin(number), end(number)}});

        find_adj_nums(nums[idx], syms[idx]);
        find_adj_nums(nums[idx], syms[abs(idx - 1) % syms.size()]);
        find_adj_nums(nums[abs(idx - 1) % nums.size()], syms[idx]);

        for (const auto &sym : syms[abs(idx - 1) % nums.size()]) {
            sum += accumulate(begin(sym.adj_nums), end(sym.adj_nums), 0);
            if (sym.s == '*' && sym.adj_nums.size() > 1) {
                pow += accumulate(begin(sym.adj_nums), end(sym.adj_nums), 1,
                                  multiplies<int>());
            }
        }

        nums[abs(idx - 1) % nums.size()].clear();
        syms[abs(idx - 1) % syms.size()].clear();
        idx = (idx + 1) % syms.size();
    }

    cout << "Part1 " << sum << "\n";
    cout << "Part2 " << pow << "\n";
}
