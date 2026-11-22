// 参考实现：c-sparse-matrix
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int r;
    int c;
    int v;
} Term;

static int cmp_term(const void *a, const void *b) {
    const Term *x = (const Term *)a;
    const Term *y = (const Term *)b;
    if (x->r != y->r) return x->r < y->r ? -1 : 1;
    if (x->c != y->c) return x->c < y->c ? -1 : 1;
    return 0;
}

int main(void) {
    int m = 0;
    int n = 0;
    int k = 0;
    if (scanf("%d %d %d", &m, &n, &k) != 3) return 0;
    if (k < 0) return 0;

    Term *terms = NULL;
    if (k > 0) {
        terms = (Term *)malloc((size_t)k * sizeof *terms);
        if (terms == NULL) return 1;
        for (int i = 0; i < k; ++i) {
            if (scanf("%d %d %d", &terms[i].r, &terms[i].c, &terms[i].v) != 3) {
                free(terms);
                return 1;
            }
        }
        qsort(terms, (size_t)k, sizeof *terms, cmp_term);
    }

    printf("%d %d %d\n", n, m, k);
    for (int c = 0; c < n; ++c) {
        for (int i = 0; i < k; ++i) {
            if (terms[i].c == c) {
                printf("%d %d %d\n", terms[i].c, terms[i].r, terms[i].v);
            }
        }
    }
    free(terms);
    return 0;
}
