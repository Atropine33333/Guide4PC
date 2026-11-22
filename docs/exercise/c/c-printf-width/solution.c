// 参考实现：c-printf-width
#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    printf("%-8s%5s\n", "name", "score");

    for (int i = 0; i < n; ++i) {
        char name[16];
        int score = 0;
        if (scanf("%15s %d", name, &score) != 2) break;
        printf("%-8s%5d\n", name, score);
    }
    return 0;
}
