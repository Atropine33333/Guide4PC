// 参考实现：c-flex-array
#include <stddef.h>
#include <stdlib.h>

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
    if (n > ((size_t)-1 - sizeof(IntFlex)) / sizeof(int)) return NULL;
    IntFlex *v = (IntFlex *)calloc(1, sizeof(IntFlex) + n * sizeof(int));
    if (v == NULL) return NULL;
    v->n = n;
    return v;
}

IntFlex *flex_from(const int *src, size_t n) {
    if (src == NULL && n != 0) return NULL;
    IntFlex *v = flex_new(n);
    if (v == NULL) return NULL;
    for (size_t i = 0; i < n; ++i) v->a[i] = src[i];
    return v;
}

int flex_get(const IntFlex *v, size_t i) {
    if (v == NULL || i >= v->n) return 0;
    return v->a[i];
}

void flex_set(IntFlex *v, size_t i, int value) {
    if (v == NULL || i >= v->n) return;
    v->a[i] = value;
}

long long flex_sum(const IntFlex *v) {
    if (v == NULL) return 0;
    long long total = 0;
    for (size_t i = 0; i < v->n; ++i) total += v->a[i];
    return total;
}

void flex_free(IntFlex *v) {
    free(v);
}
