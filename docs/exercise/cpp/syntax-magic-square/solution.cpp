#include <vector>

std::vector<int> magic_square(int n) {
    if (n <= 0 || n % 2 == 0) {
        return {};
    }

    std::vector<int> grid(static_cast<std::size_t>(n) * n, 0);
    int row = 0;
    int col = n / 2;
    for (int value = 1; value <= n * n; ++value) {
        grid[static_cast<std::size_t>(row) * n + col] = value;

        int next_row = (row - 1 + n) % n;
        int next_col = (col + 1) % n;
        if (grid[static_cast<std::size_t>(next_row) * n + next_col] != 0) {
            next_row = (row + 1) % n;
            next_col = col;
        }
        row = next_row;
        col = next_col;
    }
    return grid;
}
