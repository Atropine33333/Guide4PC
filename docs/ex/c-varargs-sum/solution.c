// 参考实现：c-varargs-sum
#include <stdarg.h>

int sum_ints(int count, ...) {
    int total = 0;
    va_list ap;

    va_start(ap, count);
    for (int i = 0; i < count; ++i) {
        total += va_arg(ap, int);
    }
    va_end(ap);

    return total;
}
