// 参考实现：c-do-while-read
#include <stdio.h>

int main(void) {
    long long sum = 0;
    int count = 0;
    int value = 0;

    do {
        if (scanf("%d", &value) != 1) break;
        if (value != 0) {
            sum += value;
            ++count;
        }
    } while (value != 0);

    printf("%d %lld\n", count, sum);
    return 0;
}
