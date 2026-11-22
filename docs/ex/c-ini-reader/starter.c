#include <stdio.h>
#include <string.h>

#define MAX_ENTRIES 100
#define KEY_MAX 32
#define VAL_MAX 64
#define LINE_MAX 200

int main(void) {
    char line[LINE_MAX + 2];
    char keys[MAX_ENTRIES][KEY_MAX];
    char vals[MAX_ENTRIES][VAL_MAX];
    int count = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        /* TODO: 去掉行首行尾的空格与制表符；
           跳过空行、以 # 开头的行、没有 = 的行；
           在第一个 = 处切开，键与值各自去掉两边空白；
           键为空的行跳过；重复的键用后来的值覆盖。 */
        (void)keys;
        (void)vals;
        (void)count;
    }

    /* TODO: 按 strcmp 给键排序，然后每行打印 key=value。 */
    return 0;
}
