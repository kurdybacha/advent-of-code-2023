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
                if (g.matrix[row][col] == Node{Node::Galaxy}) {
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
            size_t empty_rows_between = 0;
            size_t empty_cols_between = 0;
            size_t row1 = min(pos1.row, pos2.row);
            size_t row2 = max(pos1.row, pos2.row);
            size_t col1 = min(pos1.col, pos2.col);
            size_t col2 = max(pos1.col, pos2.col);
            for (size_t empty_row : empty_rows) {
                if (empty_row > row1 && empty_row < row2) {
                    empty_rows_between += expand_factor - 1;
                }
            }
            for (size_t empty_col : empty_cols) {
                if (empty_col > col1 && empty_col < col2) {
                    empty_cols_between += expand_factor - 1;
                }
            }
            return (row2 - row1 + empty_rows_between) +
                   (col2 - col1 + empty_cols_between);
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
    vector<int> empty_cols;
};

int main(int argc, char** argv) {
    Graph g = Graph::from_file(argv[1]);
    cout << format("Part 1: {}\n", g.shortest_paths(2));
    cout << format("Part 2: {}\n", g.shortest_paths(1e6));
}
