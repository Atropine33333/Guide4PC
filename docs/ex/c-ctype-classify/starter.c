#include <stdio.h>
#include <string.h>

int main(void) {
    char line[1005];
    if (fgets(line, sizeof line, stdin) == NULL) line[0] = '\0';
    line[strcspn(line, "\r\n")] = '\0';

    int letters = 0;
    int digits = 0;
    int spaces = 0;
    int others = 0;
    for (int i = 0; line[i] != '\0'; ++i) {
        /* TODO: 把 line[i] 归入四类中的一类。 */
        (void)others;
    }

    printf("%d %d %d %d\n", letters, digits, spaces, others);
    return 0;
}
