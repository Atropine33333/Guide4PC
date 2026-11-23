#include <stdio.h>
#include <string.h>

int main(void) {
    char text[1005];
    char set[1005];
    if (fgets(text, sizeof text, stdin) == NULL) return 0;
    if (fgets(set, sizeof set, stdin) == NULL) set[0] = '\0';

    text[strcspn(text, "\r\n")] = '\0';
    set[strcspn(set, "\r\n")] = '\0';

    int count = 0;
    for (int i = 0; text[i] != '\0'; ++i) {
        /* TODO: 若 text[i] 在 set 中，count 加一；否则跳出。 */
        (void)set;
        break;
    }

    printf("%d\n", count);
    return 0;
}
