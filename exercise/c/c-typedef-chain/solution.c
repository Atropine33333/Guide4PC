// 参考实现：c-typedef-chain
#include <stdio.h>

typedef int score;
typedef score *score_ptr;
typedef score_ptr score_row;

int main(void) {
    int n = 0;
    static score data[1005];
    if (scanf("%d", &n) != 1 || n < 1) return 0;
    for (int i = 0; i < n && i < 1005; ++i) {
        if (scanf("%d", &data[i]) != 1) return 0;
    }

    score_row row = data;
    score_ptr p = row + n / 2;
    printf("%zu %d\n", sizeof(score), *p);
    return 0;
}
