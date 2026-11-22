#include <stdio.h>

int main(void) {
    char line[1100];

    while (fgets(line, sizeof line, stdin) != NULL) {
        /* TODO：判断这一行是不是空行，把连续的空行压成一条，再去掉首尾的空行。 */
        fputs(line, stdout);
    }
    return 0;
}
