// 参考实现：c-2d-array
#include <stddef.h>
#include <stdlib.h>

int **make2d(int rows, int cols);
long long sum2d(int **grid, int rows, int cols);
void free2d(int **grid, int rows);

int **make2d(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;
    int **grid = (int **)malloc((size_t)rows * sizeof *grid);
    if (grid == NULL) return NULL;
    for (int r = 0; r < rows; ++r) grid[r] = NULL;
    for (int r = 0; r < rows; ++r) {
        grid[r] = (int *)calloc((size_t)cols, sizeof *grid[r]);
        if (grid[r] == NULL) {
            free2d(grid, r);
            return NULL;
        }
    }
    return grid;
}

long long sum2d(int **grid, int rows, int cols) {
    if (grid == NULL || rows <= 0 || cols <= 0) return 0;
    long long total = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) total += grid[r][c];
    }
    return total;
}

void free2d(int **grid, int rows) {
    if (grid == NULL) return;
    for (int r = 0; r < rows; ++r) free(grid[r]);
    free(grid);
}
