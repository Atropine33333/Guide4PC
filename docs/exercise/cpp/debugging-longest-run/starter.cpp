#include <algorithm>
#include <iostream>
#include <string>

int main() {
    std::string line;
    std::getline(std::cin, line);

    int best = 0;
    int run = 1;
    for (std::size_t i = 1; i < line.size(); ++i) {
        if (line[i] == line[i - 1]) {
            ++run;
        } else {
            best = std::max(best, run);
            run = 1;
        }
    }

    std::cout << best << '\n';
    return 0;
}
