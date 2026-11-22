#include <stddef.h>

typedef struct {
    size_t n;
    int a[];
} IntFlex;

IntFlex *flex_new(size_t n);
IntFlex *flex_from(const int *src, size_t n);
int flex_get(const IntFlex *v, size_t i);
void flex_set(IntFlex *v, size_t i, int value);
long long flex_sum(const IntFlex *v);
void flex_free(IntFlex *v);

IntFlex *flex_new(size_t n) {
    /* TODO: 一次申请 sizeof(IntFlex) + n * sizeof(int) 字节，n 记下数目，
       所有格子置 0。申请失败返回 NULL。 */
    (void)n;
    return NULL;
}

IntFlex *flex_from(const int *src, size_t n) {
    /* TODO: 新建 n 格再逐个搬入 src 的值。 */
    (void)src;
    (void)n;
    return NULL;
}

int flex_get(const IntFlex *v, size_t i) {
    /* TODO: 返回第 i 格；越界或 v 为 NULL 返回 0。 */
    (void)v;
    (void)i;
    return 0;
}

void flex_set(IntFlex *v, size_t i, int value) {
    /* TODO: 改写第 i 格；越界时什么也不做。 */
    (void)v;
    (void)i;
    (void)value;
}

long long flex_sum(const IntFlex *v) {
    /* TODO */
    (void)v;
    return 0;
}

void flex_free(IntFlex *v) {
    /* TODO: 归还整块内存。 */
    (void)v;
}
