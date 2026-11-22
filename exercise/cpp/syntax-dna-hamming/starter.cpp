#include <iostream>
#include <string>

int main() {
    std::string first;
    std::string second;
    std::cin >> first >> second;

    int distance = 0;
    for (std::size_t i = 0; i < first.size(); ++i) {
        // TODO: count positions where the characters differ.
        if (first[i] == 'A' && second[i] == 'A') ++distance;
    }

    // TODO: build the reverse complement instead of copying the strand.
    std::string reverse_complement = first;

    std::cout << distance << '\n' << reverse_complement << '\n';
    return 0;
}
