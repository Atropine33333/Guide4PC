#include <stdio.h>

typedef int score;
typedef score *score_ptr;

int main(void) {
    int n = 0;
    score data[1005];
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n && i < 1005; ++i) {
        if (scanf("%d", &data[i]) != 1) return 0;
    }

    /* TODO: 让 score_ptr 指向下标 n / 2 的元素，
       然后输出 sizeof(score) 与 *p，用一个空格隔开。 */
    printf("0 0\n");
    return 0;
}
