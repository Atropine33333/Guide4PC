// 参考实现：c-printf-table
#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0 || n > 50) return 0;

    printf("%-8s  %5s\n", "name", "score");

    for (int i = 0; i < n; ++i) {
        char name[11];
        int score;
        if (scanf("%10s %d", name, &score) != 2) return 0;

        printf("%-8s  %5d\n", name, score);
    }
    return 0;
}
