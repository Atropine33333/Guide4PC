#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 205

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* 吃掉第一行剩下的换行符。 */
    }

    char line[MAX_LINE];
    for (int i = 0; i < n; ++i) {
        if (fgets(line, sizeof line, stdin) == NULL) break;

        /* TODO: 去掉行首尾空白；空行与以 # 开头的行跳过；
           在第一个 = 处拆出键和值，分别去掉首尾空白；
           按 "key: value" 输出。 */
        (void)line;
    }

    return 0;
}
