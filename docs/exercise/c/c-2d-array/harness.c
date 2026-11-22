// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        int **grid = make2d(3, 4);
        int ok = grid != NULL;
        if (ok) {
            ok = grid[0] != NULL && grid[1] != NULL && grid[2] != NULL;
            ok = ok && grid[0] != grid[1] && grid[1] != grid[2];
            for (int r = 0; ok && r < 3; ++r)
                for (int c = 0; ok && c < 4; ++c)
                    if (grid[r][c] != 0) ok = 0;
            ok = ok && sum2d(grid, 3, 4) == 0;
        }
        free2d(grid, 3);
        report("shape-and-zeros", ok);
    }
    {
        int **grid = make2d(3, 3);
        int ok = grid != NULL;
        if (ok) {
            int value = -5;
            for (int r = 0; r < 3; ++r)
                for (int c = 0; c < 3; ++c) grid[r][c] = value++;
            ok = grid[0][0] == -5 && grid[2][2] == 3;
            ok = ok && sum2d(grid, 3, 3) == (-5 + -4 + -3 + -2 + -1 + 0 + 1 + 2 + 3);
        }
        free2d(grid, 3);
        report("fill-and-sum", ok);
    }
    {
        int **grid = make2d(4, 2);
        int ok = grid != NULL;
        if (ok) {
            grid[0][0] = 11;
            grid[0][1] = 12;
            ok = grid[1][0] == 0 && grid[2][0] == 0 && grid[3][1] == 0;
            grid[3][1] = 30;
            ok = ok && grid[0][1] == 12 && grid[1][1] == 0;
            ok = ok && sum2d(grid, 4, 2) == 53;
        }
        free2d(grid, 4);
        report("rows-independent", ok);
    }
    {
        int ok = make2d(0, 4) == NULL && make2d(4, 0) == NULL;
        ok = ok && make2d(-2, 3) == NULL && make2d(3, -1) == NULL;
        report("bad-size", ok);
    }
    {
        int **first = make2d(2, 2);
        int ok = first != NULL;
        free2d(first, 2);
        free2d(NULL, 0);
        int **second = make2d(2, 2);
        ok = ok && second != NULL && second[0][0] == 0 && second[1][1] == 0;
        ok = ok && sum2d(second, 2, 2) == 0;
        free2d(second, 2);
        report("free-then-reuse", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
