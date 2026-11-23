// 参考实现：c-enum-ordinal
#include <stdio.h>

enum E { A, B = 5, C, D = 2 };

static int ordinal(char letter) {
    switch (letter) {
        case 'A': return A;
        case 'B': return B;
        case 'C': return C;
        case 'D': return D;
        default: return 0;
    }
}

int main(void) {
    char word[16];
    if (scanf("%15s", word) != 1) return 0;

    for (int i = 0; word[i] != '\0'; ++i) {
        if (i > 0) putchar(' ');
        printf("%d", ordinal(word[i]));
    }
    putchar('\n');
    return 0;
}
