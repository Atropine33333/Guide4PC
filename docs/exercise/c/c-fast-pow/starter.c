#include <stdio.h>

int main(void) {
    long long b = 0;
    long long e = 0;
    long long m = 0;
    if (scanf("%lld %lld %lld", &b, &e, &m) != 3) return 0;

    long long result = 1 % m;

    while (e > 0) {
        /* TODO: 指数末位为 1 时，把 b 乘进 result 并立刻取模。 */
        /* TODO: b 自乘后取模。 */
        /* TODO: e 折半。 */
        e = 0; /* 占位：补全上面三行后删掉这一行。 */
    }

    printf("%lld\n", result);
    return 0;
}
