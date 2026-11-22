#include <iostream>
#include <map>
#include <string>

int main() {
    std::string text;
    std::getline(std::cin, text);

    // TODO: build the Huffman tree and emit the encoded text and code table.
    std::cout << text << '\n';
    std::map<char, int> frequency;
    for (char ch : text) ++frequency[ch];
    for (const auto& entry : frequency) {
        std::cout << entry.first << " 0\n";
    }
    return 0;
}
