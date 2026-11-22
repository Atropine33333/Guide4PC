#include <stdio.h>

/* TODO: 用宏拼出三段版本号的格式串，段与段之间用一个点。 */
#define VERSION_FMT "%d.%d"

int main(void) {
    int major = 0;
    int minor = 0;
    int patch = 0;
    if (scanf("%d %d %d", &major, &minor, &patch) != 3) return 0;

    printf(VERSION_FMT "\n", major, minor);
    (void)patch;
    return 0;
}
