#include <stddef.h>

int sum_array(const int *a, int n) {
    /* BUG: 循环多走了一格。 */
    int total = 0;
    for (int i = 0; i <= n; ++i) {
        total += a[i];
    }
    return total;
}

int last_value(const int *a, int n) {
    /* BUG: 收尾的时候多读了数组后面的一格。 */
    int last = 0;
    for (int i = 0; i <= n; ++i) {
        last = a[i];
    }
    return last;
}
