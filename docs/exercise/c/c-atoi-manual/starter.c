#include <stdio.h>

int main(void) {
    char line[256];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    long long value = 0;
    /* TODO: 跳过前导空格，处理可选的正负号，再把数字字符累加进 value。 */
    printf("%lld\n", value);
    return 0;
}
