// 参考实现：c-letter-camp
#include <stdio.h>

int main(void) {
    char word[1005];
    if (scanf("%1004s", word) != 1) return 0;

    int upper = 0;
    int lower = 0;
    int digits = 0;

    for (int i = 0; word[i] != '\0'; ++i) {
        char ch = word[i];
        if (ch >= 'A' && ch <= 'Z') {
            ++upper;
        } else if (ch >= 'a' && ch <= 'z') {
            ++lower;
        } else if (ch >= '0' && ch <= '9') {
            ++digits;
        }
    }

    printf("%d %d %d\n", upper, lower, digits);
    return 0;
}
