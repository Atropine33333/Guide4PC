#include <stddef.h>

void safe_free(void **p) {
    /* TODO: p 或 *p 为 NULL 时什么也不做；否则归还 *p 并把 *p 置为 NULL。 */
    (void)p;
}
