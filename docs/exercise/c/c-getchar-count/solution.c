// 参考实现：c-getchar-count
#include <stdio.h>
#include <ctype.h>

int main(void) {
    long chars = 0;
    long lines = 0;
    long words = 0;
    int in_word = 0;
    int c;

    while ((c = getchar()) != EOF) {
        ++chars;
        if (c == '\n') ++lines;
        if (isspace((unsigned char)c)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            ++words;
        }
    }

    printf("%ld %ld %ld\n", chars, lines, words);
    return 0;
}
