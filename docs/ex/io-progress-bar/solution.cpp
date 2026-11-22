#include <iostream>

int main() {
    int percent = 0;
    int width = 0;
    std::cin >> percent >> width;

    const int filled = percent * width / 100;
    std::cout << '[';
    for (int i = 0; i < width; ++i) {
        std::cout << (i < filled ? '#' : '-');
    }
    std::cout << "] " << percent << "%\n";
    return 0;
}
