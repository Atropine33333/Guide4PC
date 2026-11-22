// 参考实现：c-snprintf-format
#include <stdio.h>

int main(void) {
    char name[40];
    int age = 0;
    if (scanf("%39s %d", name, &age) != 2) return 0;

    char line[80];
    snprintf(line, sizeof line, "name=%s; age=%d", name, age);

    printf("%s\n", line);
    return 0;
}
