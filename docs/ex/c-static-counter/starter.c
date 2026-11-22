#include <stddef.h>

/* TODO: 在这里放一个文件作用域的 static 计数器。 */

void counter_reset(void);
int counter_next(void);
int counter_add(int delta);
int counter_get(void);

void counter_reset(void) {
    /* TODO: 把计数器归零。 */
}

int counter_next(void) {
    /* TODO: 计数器加一，返回加完后的值。 */
    return 0;
}

int counter_add(int delta) {
    /* TODO: 计数器加上 delta，返回加完后的值。 */
    (void)delta;
    return 0;
}

int counter_get(void) {
    /* TODO: 返回当前值，不要改动它。 */
    return 0;
}
