// 参考实现：c-bug-array
#include <stddef.h>

int sum_array(const int *a, int n) {
    int total = 0;
    for (int i = 0; i < n; ++i) {
        total += a[i];
    }
    return total;
}

int last_value(const int *a, int n) {
    return a[n - 1];
}
