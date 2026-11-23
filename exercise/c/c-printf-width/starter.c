#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    /* TODO: 表头也用同一套列宽：name 左对齐 8 列，score 右对齐 5 列。 */
    printf("%s %s\n", "name", "score");

    for (int i = 0; i < n; ++i) {
        char name[16];
        int score = 0;
        if (scanf("%15s %d", name, &score) != 2) break;
        /* TODO: 按上面的列宽把一行排好再输出。 */
        printf("%s %d\n", name, score);
    }
    return 0;
}
