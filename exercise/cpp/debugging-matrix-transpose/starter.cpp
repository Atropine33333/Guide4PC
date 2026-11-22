#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) std::cin >> matrix[i][j];
    }

    // BUG: these loops print the original matrix, not its transpose.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (j != 0) std::cout << ' ';
            std::cout << matrix[i][j];
        }
        std::cout << '\n';
    }
    return 0;
}
