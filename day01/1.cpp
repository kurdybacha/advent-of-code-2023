#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>

using namespace std;

int main(int , char **) {
    fstream f{"input.txt"};
    string line;
    uint64_t sum = 0;
    while (getline(f, line)) {
        auto f = find_if(begin(line), end(line), ::isdigit);
        auto l = find_if(rbegin(line), rend(line), ::isdigit);
        sum += (*f - '0') * 10 + (*l - '0');
    }
    std::cout << sum << std::endl;
    return 0;
}
