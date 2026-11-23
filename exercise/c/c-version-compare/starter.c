#include <stdio.h>
#include <string.h>

#define MAX_LINE 128
#define MAX_SEG 4

int main(void) {
    char first[MAX_LINE];
    char second[MAX_LINE];
    if (fgets(first, sizeof first, stdin) == NULL) return 0;
    if (fgets(second, sizeof second, stdin) == NULL) return 0;

    first[strcspn(first, " \t\r\n")] = '\0';
    second[strcspn(second, " \t\r\n")] = '\0';

    /* TODO: 把两行各拆成至多 4 段整数，缺段补 0，
       从左往右逐段比较，输出 -1、0 或 1。 */
    (void)first;
    (void)second;

    return 0;
}
