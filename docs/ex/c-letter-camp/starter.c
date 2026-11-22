#include <stdio.h>

int main(void) {
    char word[1005];
    if (scanf("%1004s", word) != 1) return 0;

    int upper = 0;
    int lower = 0;
    int digits = 0;

    for (int i = 0; word[i] != '\0'; ++i) {
        /* TODO: 按字符范围把 word[i] 记进三类之一。 */
        (void)word[i];
    }

    printf("%d %d %d\n", upper, lower, digits);
    return 0;
}
