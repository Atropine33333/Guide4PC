// 参考实现：c-version-compare
#include <stdio.h>
#include <string.h>

#define MAX_LINE 128
#define MAX_SEG 4

static void parse_version(const char *text, long long seg[MAX_SEG]) {
    for (int i = 0; i < MAX_SEG; ++i) {
        seg[i] = 0;
    }

    int index = 0;
    const char *p = text;
    while (index < MAX_SEG) {
        long long value = 0;
        int digits = 0;
        while (*p >= '0' && *p <= '9') {
            value = value * 10 + (*p - '0');
            ++p;
            ++digits;
        }
        if (digits == 0) break;
        seg[index++] = value;
        if (*p != '.') break;
        ++p;
    }
}

int main(void) {
    char first[MAX_LINE];
    char second[MAX_LINE];
    if (fgets(first, sizeof first, stdin) == NULL) return 0;
    if (fgets(second, sizeof second, stdin) == NULL) return 0;

    first[strcspn(first, " \t\r\n")] = '\0';
    second[strcspn(second, " \t\r\n")] = '\0';

    long long a[MAX_SEG];
    long long b[MAX_SEG];
    parse_version(first, a);
    parse_version(second, b);

    int result = 0;
    for (int i = 0; i < MAX_SEG; ++i) {
        if (a[i] < b[i]) {
            result = -1;
            break;
        }
        if (a[i] > b[i]) {
            result = 1;
            break;
        }
    }

    printf("%d\n", result);
    return 0;
}
