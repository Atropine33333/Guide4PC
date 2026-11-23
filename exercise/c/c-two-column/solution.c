// 参考实现：c-two-column
#include <stdio.h>
#include <string.h>

static int is_blank(const char *line) {
    for (const char *p = line; *p != '\0'; ++p) {
        if (*p != ' ' && *p != '\t') return 0;
    }
    return 1;
}

int main(void) {
    char line[256];
    long long rows = 0;
    long long sum = 0;
    long long skipped = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }
        if (is_blank(line)) continue;

        int a = 0;
        int b = 0;
        if (sscanf(line, "%d %d", &a, &b) != 2) {
            ++skipped;
            continue;
        }

        long long s = (long long)a + (long long)b;
        printf("%d %d %lld\n", a, b, s);
        ++rows;
        sum += s;
    }

    printf("total %lld sum %lld skipped %lld\n", rows, sum, skipped);
    return 0;
}
