#include <stdio.h>

int main(void) {
    int score = 0;
    if (scanf("%d", &score) != 1) return 0;

    /* TODO: 用嵌套三目表达式从高分档往低分档判，得到 A、B、C、D、F。 */
    char grade = '-';

    printf("%c\n", grade);
    return 0;
}
