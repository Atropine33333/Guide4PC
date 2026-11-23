// 参考实现：c-ternary-grade
#include <stdio.h>

int main(void) {
    int score = 0;
    if (scanf("%d", &score) != 1) return 0;

    char grade = score >= 90 ? 'A'
               : score >= 80 ? 'B'
               : score >= 70 ? 'C'
               : score >= 60 ? 'D'
                             : 'F';

    printf("%c\n", grade);
    return 0;
}
