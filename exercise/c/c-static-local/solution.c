// 参考实现：c-static-local
#include <stddef.h>

/* 静态存储期的计数器：函数返回后仍然留在原地。 */
static int last_id = 0;

int next_id(void) {
    return ++last_id;
}

void reset_ids(void) {
    last_id = 0;
}
