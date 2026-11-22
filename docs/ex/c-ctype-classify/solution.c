// 参考实现：c-ctype-classify
#include <ctype.h>
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
        unsigned char ch = (unsigned char)line[i];
        if (isalpha(ch)) {
            ++letters;
        } else if (isdigit(ch)) {
            ++digits;
        } else if (isspace(ch)) {
            ++spaces;
        } else {
            ++others;
        }
    }

    printf("%d %d %d %d\n", letters, digits, spaces, others);
    return 0;
}
