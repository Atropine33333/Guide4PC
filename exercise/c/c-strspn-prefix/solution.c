// 参考实现：c-strspn-prefix
#include <stdio.h>
#include <string.h>

int main(void) {
    char text[1005];
    char set[1005];
    if (fgets(text, sizeof text, stdin) == NULL) return 0;
    if (fgets(set, sizeof set, stdin) == NULL) set[0] = '\0';

    text[strcspn(text, "\r\n")] = '\0';
    set[strcspn(set, "\r\n")] = '\0';

    size_t count = strspn(text, set);
    printf("%zu\n", count);
    return 0;
}
