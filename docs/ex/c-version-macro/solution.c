// 参考实现：c-version-macro
#include <stdio.h>

#define VERSION_FMT "%d.%d.%d"

int main(void) {
    int major = 0;
    int minor = 0;
    int patch = 0;
    if (scanf("%d %d %d", &major, &minor, &patch) != 3) return 0;

    printf(VERSION_FMT "\n", major, minor, patch);
    return 0;
}
