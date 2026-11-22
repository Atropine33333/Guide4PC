// Reference solution: syntax-dna-hamming
#include <iostream>
#include <string>

int main() {
    std::string first;
    std::string second;
    if (!(std::cin >> first >> second)) return 0;

    int distance = 0;
    for (std::size_t i = 0; i < first.size(); ++i) {
        if (first[i] != second[i]) ++distance;
    }

    std::string reverse_complement;
    reverse_complement.reserve(first.size());
    for (std::string::reverse_iterator it = first.rbegin(); it != first.rend(); ++it) {
        char base = 'A';
        switch (*it) {
            case 'A': base = 'T'; break;
            case 'T': base = 'A'; break;
            case 'C': base = 'G'; break;
            case 'G': base = 'C'; break;
            default: base = *it; break;
        }
        reverse_complement.push_back(base);
    }

    std::cout << distance << '\n' << reverse_complement << '\n';
    return 0;
}
