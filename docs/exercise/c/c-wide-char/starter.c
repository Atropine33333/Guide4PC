#include <stdio.h>
#include <wchar.h>

int main(void) {
    char line[1005];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    /* TODO: 去掉行末换行，逐个字符转成 wchar_t，
       输出它的十进制值，值与值之间空一格。 */
    printf("0\n");
    return 0;
}
