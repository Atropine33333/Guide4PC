#include <stdio.h>
#include <string.h>

#define NUM_MAX 1000

int main(void) {
    char a[NUM_MAX + 1];
    char b[NUM_MAX + 1];
    if (scanf("%1000s", a) != 1) return 0;
    if (scanf("%1000s", b) != 1) return 0;

    /* TODO: 从最低位开始逐位相加，把进位记下来；
       结果先倒着放进缓冲区，最后再正着输出。 */

    printf("0\n");
    return 0;
}
