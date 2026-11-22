#include <stdio.h>
#include <string.h>

typedef long long (*BinOp)(long long, long long);

static long long op_add(long long a, long long b) {
    /* TODO: 返回 a + b。 */
    (void)a;
    (void)b;
    return 0;
}

static long long op_sub(long long a, long long b) {
    /* TODO: 返回 a - b。 */
    (void)a;
    (void)b;
    return 0;
}

static long long op_mul(long long a, long long b) {
    /* TODO: 返回 a * b。 */
    (void)a;
    (void)b;
    return 0;
}

static long long op_max(long long a, long long b) {
    /* TODO: 返回两者中较大的一个。 */
    (void)a;
    (void)b;
    return 0;
}

typedef struct {
    const char *name;
    BinOp fn;
} Card;

int main(void) {
    char name[16];
    long long a = 0;
    long long b = 0;
    if (scanf("%15s %lld %lld", name, &a, &b) != 3) return 0;

    static const Card cards[] = {
        { "+", op_add },
        { "-", op_sub },
        { "*", op_mul },
        { "max", op_max },
    };
    (void)cards;

    /* TODO: 在 cards 表里按名字找到对应的函数指针；找不到就让 fn 保持 NULL。 */
    BinOp fn = NULL;

    if (fn == NULL) {
        printf("?\n");
        return 0;
    }
    printf("%lld\n", fn(a, b));
    return 0;
}
