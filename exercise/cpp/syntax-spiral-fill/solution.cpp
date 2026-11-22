// 参考实现：syntax-spiral-fill
#include <iostream>
#include <string>
#include <vector>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::vector<std::vector<int>> grid(n, std::vector<int>(n, 0));
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    int row = 0;
    int col = 0;
    int direction = 0;
    for (int value = 1; value <= n * n; ++value) {
        grid[row][col] = value;
        const int nr = row + dr[direction];
        const int nc = col + dc[direction];
        if (nr < 0 || nr >= n || nc < 0 || nc >= n || grid[nr][nc] != 0) {
            direction = (direction + 1) % 4;
        }
        row += dr[direction];
        col += dc[direction];
    }

    const int width = static_cast<int>(std::to_string(n * n).size());
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (c > 0) std::cout << ' ';
            const std::string text = std::to_string(grid[r][c]);
            for (int pad = width - static_cast<int>(text.size()); pad > 0; --pad) std::cout << ' ';
            std::cout << text;
        }
        std::cout << '\n';
    }
    return 0;
}
