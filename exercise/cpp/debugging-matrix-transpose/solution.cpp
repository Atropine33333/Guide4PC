// Reference solution: debugging-matrix-transpose
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

    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            if (i != 0) std::cout << ' ';
            std::cout << matrix[i][j];
        }
        std::cout << '\n';
    }
    return 0;
}
