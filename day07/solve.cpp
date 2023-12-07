#include <assert.h>
#include <bits/ranges_cmp.h>

#include <algorithm>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <vector>

using namespace std;

struct Card {
    char c;
    int strength(bool with_joker) const {
        switch (c) {
            case 'T':
                return 10;
            case 'J':
                return with_joker ? 1 : 11;
            case 'Q':
                return 12;
            case 'K':
                return 13;
            case 'A':
                return 14;
            default:
                return c - '0';
        }
    }
};
bool comp(const Card &a, const Card &b, bool with_joker = false) {
    return a.strength(with_joker) < b.strength(with_joker);
}

bool operator<(const Card &a, const Card &b) { return comp(a, b, false); }

struct Hand {
    enum HandType : int {
        Unknown = 0,
        HighCard,
        OnePair,
        TwoPair,
        ThreeKind,
        FullHouse,
        FourKind,
        FiveKind,
    };
    Hand(string_view sv) {
        for (const auto &[idx, c] : views::enumerate(sv)) {
            cards[idx] = Card{c};
        }
    }
    array<Card, 5> cards;

    HandType type_from_counts(const vector<int> &vec) const {
        HandType type = Unknown;
        switch (vec.size()) {
            case 5:
                type = HighCard;
                break;
            case 4:
                type = OnePair;
                break;
            case 3: {
                if (vec[0] == 3) {
                    type = ThreeKind;
                } else if (vec[0] == 2) {
                    type = TwoPair;
                }
            } break;
            case 2: {
                if (vec[0] == 4) {
                    type = FourKind;
                } else if (vec[0] == 3) {
                    type = FullHouse;
                }
            } break;
            case 1:
                type = FiveKind;
                break;
            default:
                assert(false);
        }
        assert(type != Unknown);
        return type;
    }

    int strength(bool with_joker) const {
        map<Card, int> counts = {};
        for (const auto &[idx, c] : views::enumerate(cards)) {
            counts[c] += 1;
        }
        auto v = counts | views::values;
        vector<int> vec{begin(v), end(v)};

        sort(begin(vec), end(vec), greater<int>());
        if (with_joker) {
            int jcount = counts[Card('J')];
            auto it = find(begin(vec), end(vec), jcount);
            if (it != end(vec)) {
                vec.erase(it);
            }
            if (vec.empty()) {
                vec.push_back(jcount);
            } else {
                vec[0] += jcount;
            }
        }
        return type_from_counts(vec);
    }
};

bool comp(const Hand &a, const Hand &b, bool with_joker) {
    int a_s = a.strength(with_joker);
    int b_s = b.strength(with_joker);
    if (a_s == b_s) {
        for (int i = 0; i < a.cards.size(); ++i) {
            if (a.cards[i].c != b.cards[i].c) {
                return comp(a.cards[i], b.cards[i], with_joker);
            }
        }
    }
    return a_s < b_s;
}

int main(int argc, char **argv) {
    fstream file{argv[1]};
    string line;
    vector<pair<Hand, ulong>> hands;
    while (getline(file, line)) {
        auto hand = line.substr(0, line.find_first_of(' '));
        ulong rank = stol(line.substr(line.find_first_of(' ') + 1));
        hands.push_back({Hand{hand}, rank});
    }

    sort(begin(hands), end(hands), [](const auto &p1, const auto &p2) {
        return comp(p1.first, p2.first, false);
    });

    auto sum = [](const decltype(hands) &v) {
        ulong s = 0;
        for (const auto &[idx, p] : views::enumerate(v)) {
            s += (idx + 1) * p.second;
        }
        return s;
    };
    cout << "Part 1: " << sum(hands) << "\n";

    sort(begin(hands), end(hands), [](const auto &p1, const auto &p2) {
        return comp(p1.first, p2.first, true);
    });
    cout << "Part 2: " << sum(hands) << "\n";
}
