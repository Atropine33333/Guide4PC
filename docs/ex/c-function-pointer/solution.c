// 参考实现：c-function-pointer
#include <stdio.h>
#include <string.h>

typedef long long (*BinOp)(long long, long long);

static long long op_add(long long a, long long b) { return a + b; }
static long long op_sub(long long a, long long b) { return a - b; }
static long long op_mul(long long a, long long b) { return a * b; }
static long long op_max(long long a, long long b) { return a > b ? a : b; }

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

    BinOp fn = NULL;
    for (size_t i = 0; i < sizeof cards / sizeof cards[0]; ++i) {
        if (strcmp(cards[i].name, name) == 0) {
            fn = cards[i].fn;
            break;
        }
    }

    if (fn == NULL) {
        printf("?\n");
        return 0;
    }
    printf("%lld\n", fn(a, b));
    return 0;
}
