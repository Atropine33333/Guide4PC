#include <stdio.h>

int main(void) {
    long chars = 0;
    long lines = 0;
    long words = 0;
    int c;

    while ((c = getchar()) != EOF) {
        ++chars;
        /* TODO: 数出换行符个数，并在空白与非空白交界处累加单词数。 */
        (void)c;
    }

    printf("%ld %ld %ld\n", chars, lines, words);
    return 0;
}
