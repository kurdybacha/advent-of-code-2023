#include <algorithm>
#include <csignal>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

using namespace std;

struct Pos {
    Pos(size_t r, size_t c) : row{r}, col{c} {}
    size_t row = 0;
    size_t col = 0;
    friend bool operator<=>(const Pos&, const Pos&) = default;
};

class Graph {
   public:
    using Node = char;
    static Graph from_file(const string& filename) {
        fstream file{filename};
        string line;
        Graph g;
        while (getline(file, line)) {
            g.nodes.emplace_back(vector<Node>{begin(line), end(line)});
            auto& row = g.nodes.back();
            auto it = ranges::find(row, 'S');
            if (it != end(row)) {
                g.start = Pos(g.nodes.size() - 1, distance(begin(row), it));
            }
        }
        return g;
    }

    bool can_try_west(Node n) const { return "S-J7"sv.contains(n); }
    bool can_try_east(Node n) const { return "S-LF"sv.contains(n); }
    bool can_try_south(Node n) const { return "S|7F"sv.contains(n); }
    bool can_try_north(Node n) const { return "S|LJ"sv.contains(n); }

    Pos pos_north(Pos pos) const {
        return Pos{pos.row > 0 ? pos.row - 1 : pos.row, pos.col};
    }
    Pos pos_south(Pos pos) const {
        return Pos{pos.row < nodes.back().size() - 1 ? pos.row + 1 : pos.row,
                   pos.col};
    }
    Pos pos_west(Pos pos) const {
        return Pos{pos.row, pos.col > 0 ? pos.col - 1 : pos.col};
    }
    Pos pos_east(Pos pos) const {
        return Pos{pos.row, pos.col < nodes.size() - 1 ? pos.col + 1 : pos.col};
    }

    const Node& node_at(Pos pos) const { return nodes[pos.row][pos.col]; }

    bool to_west_allowed(Node n) const { return "SL-F"sv.contains(n); }
    bool to_east_allowed(Node n) const { return "S-J7"sv.contains(n); }
    bool to_south_allowed(Node n) const { return "S|LJ"sv.contains(n); }
    bool to_north_allowed(Node n) const { return "S|7F"sv.contains(n); }

    Pos next(Pos pos, const vector<vector<bool>>& visited) const {
        auto ops = array{
            tuple{&Graph::pos_east, &Graph::can_try_east,
                  &Graph::to_east_allowed},
            tuple{&Graph::pos_west, &Graph::can_try_west,
                  &Graph::to_west_allowed},
            tuple{&Graph::pos_south, &Graph::can_try_south,
                  &Graph::to_south_allowed},
            tuple{&Graph::pos_north, &Graph::can_try_north,
                  &Graph::to_north_allowed},
        };
        for (const auto& [try_move, can_try_move, can_move] : ops) {
            if ((this->*can_try_move)(node_at(pos))) {
                Pos new_pos = (this->*try_move)(pos);
                if (new_pos != pos && !visited[new_pos.row][new_pos.col] &&
                    (this->*can_move)(node_at(new_pos))) {
                    return new_pos;
                }
            }
        }
        return pos;
    }

    size_t walk_the_loop() const {
        vector<vector<bool>> visited(nodes.size(),
                                     vector<bool>(nodes.front().size(), false));

        visited[start.row][start.col] = true;
        size_t steps = 0;
        auto pos = start;
        while (true) {
            visited[pos.row][pos.col] = true;
            auto new_pos = next(pos, visited);
            ++steps;
            if (new_pos == pos) break;
            pos = new_pos;
        }
        return steps;
    }

   private:
    vector<vector<Node>> nodes;
    Pos start{0, 0};
};

int main(int argc, char** argv) {
    const Graph& g = Graph::from_file(argv[1]);
    cout << format("Part 1: {}\n", g.walk_the_loop() / 2);
}
