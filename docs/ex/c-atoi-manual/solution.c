// 参考实现：c-atoi-manual
#include <stdio.h>

int main(void) {
    char line[256];
    if (fgets(line, sizeof line, stdin) == NULL) return 0;

    const char *p = line;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') ++p;

    int negative = 0;
    if (*p == '+' || *p == '-') {
        negative = (*p == '-');
        ++p;
    }

    long long value = 0;
    while (*p >= '0' && *p <= '9') {
        value = value * 10 - (*p - '0');
        ++p;
    }
    if (!negative) value = -value;

    printf("%lld\n", value);
    return 0;
}
