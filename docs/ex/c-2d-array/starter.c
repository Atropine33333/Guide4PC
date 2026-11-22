#include <stddef.h>

int **make2d(int rows, int cols);
long long sum2d(int **grid, int rows, int cols);
void free2d(int **grid, int rows);

int **make2d(int rows, int cols) {
    /* TODO: 为 rows 个行指针申请内存，再为每一行申请 cols 个 int 并置 0。
       行列不是正数或申请失败时返回 NULL。 */
    (void)rows;
    (void)cols;
    return NULL;
}

long long sum2d(int **grid, int rows, int cols) {
    /* TODO: 把所有元素加起来。 */
    (void)grid;
    (void)rows;
    (void)cols;
    return 0;
}

void free2d(int **grid, int rows) {
    /* TODO: 先归还每一行，再归还行指针数组。grid 为 NULL 时什么也不做。 */
    (void)grid;
    (void)rows;
}
