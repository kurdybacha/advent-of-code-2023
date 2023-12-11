#include <algorithm>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <vector>

using namespace std;

struct Pos {
    size_t row;
    size_t col;
};

struct Node {
    enum Type : uint8_t {
        Space = 0x1,
        Galaxy = 0x2,
        Visited = 0x4,
    };
    explicit Node(char c) {
        switch (c) {
            case '.':
                type = Space;
                break;
            case '#':
                type = Galaxy;
                break;
            default:
                throw invalid_argument("invalid tile type");
        }
    }
    explicit Node(Type t) : type{t} {}
    Type type;
    friend bool operator<=>(const Node&, const Node&) = default;
};

class Graph {
   public:
    static Graph from_file(const string& filename) {
        fstream file{filename};
        string line;
        Graph g;
        while (getline(file, line)) {
            g.matrix.emplace_back(vector<Node>{begin(line), end(line)});
            auto& row = g.matrix.back();
            auto it = find(begin(row), end(row), Node{Node::Galaxy});
            bool empty = true;
            while (it != end(row)) {
                empty = false;
                g.galaxies.emplace_back(
                    Pos(g.matrix.size() - 1, distance(begin(row), it)));
                it = find(it + 1, end(row), Node{Node::Galaxy});
            }
            if (empty) {
                g.empty_rows.emplace_back(g.matrix.size() - 1);
            }
        }
        for (size_t col = 0; col < g.matrix.front().size(); ++col) {
            bool empty = true;
            for (size_t row = 0; row < g.matrix.size(); ++row) {
                const auto& node = g.matrix[row][col];
                if (node.type == Node::Galaxy) {
                    empty = false;
                    break;
                }
            }
            if (empty) {
                g.empty_cols.emplace_back(col);
            }
        }
        return g;
    }

    size_t shortest_paths(size_t expand_factor) {
        auto shortest_path = [&](Pos pos1, Pos pos2, size_t expand_factor) {
            size_t row1 = min(pos1.row, pos2.row);
            size_t row2 = max(pos1.row, pos2.row);
            size_t col1 = min(pos1.col, pos2.col);
            size_t col2 = max(pos1.col, pos2.col);

            auto const expand = [expand_factor](
                                    size_t p1, size_t p2,
                                    const vector<size_t>& numbers) -> size_t {
                return distance(ranges::upper_bound(numbers, p1),
                                ranges::lower_bound(numbers, p2)) *
                       (expand_factor - 1);
            };
            return (row2 - row1 + expand(row1, row2, empty_rows)) +
                   (col2 - col1 + expand(col1, col2, empty_cols));
        };

        size_t sum = 0;
        for (size_t i = 0; i < galaxies.size() - 1; ++i) {
            for (size_t j = i + 1; j < galaxies.size(); ++j) {
                sum += shortest_path(galaxies[j], galaxies[i], expand_factor);
            }
        }

        return sum;
    }

   private:
    vector<vector<Node>> matrix;
    vector<Pos> galaxies;
    vector<size_t> empty_rows;
    vector<size_t> empty_cols;
};

int main(int argc, char** argv) {
    Graph g = Graph::from_file(argv[1]);
    cout << format("Part 1: {}\n", g.shortest_paths(2));
    cout << format("Part 2: {}\n", g.shortest_paths(1e6));
}
