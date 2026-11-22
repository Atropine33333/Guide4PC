#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    // TODO: 按螺旋顺序填表，而不是按行依次填数。
    std::vector<std::vector<int>> grid(n, std::vector<int>(n, 0));
    int value = 1;
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            grid[row][col] = value++;
        }
    }

    const int width = static_cast<int>(std::to_string(n * n).size());
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            if (col > 0) std::cout << ' ';
            std::cout.width(width);
            std::cout << grid[row][col];
        }
        std::cout << '\n';
    }
    return 0;
}
