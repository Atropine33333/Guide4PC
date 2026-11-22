#include <stdio.h>

int main(void) {
    char name[40];
    int age = 0;
    if (scanf("%39s %d", name, &age) != 2) return 0;

    char line[80];
    /* TODO: 用 snprintf 把 name 与 age 拼成 "name=<姓名>; age=<年龄>"。 */
    line[0] = '\0';

    printf("%s\n", line);
    return 0;
}
