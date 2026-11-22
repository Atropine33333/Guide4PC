#include <stdio.h>
#include <string.h>

int main(void) {
    char line[1100];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[--len] = '\0';

    /* TODO: 逐个字符判断是不是 \n \t \r \\ \"，是就写名字，其余原样，
       相邻两个记号之间补一个空格。 */
    (void)len;
    printf("TODO\n");
    return 0;
}
