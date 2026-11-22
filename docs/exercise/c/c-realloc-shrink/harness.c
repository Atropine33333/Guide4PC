// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
#include <stdlib.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

static int *make_seq(int n) {
    int *a = (int *)malloc((size_t)n * sizeof(int));
    if (a != NULL) {
        for (int i = 0; i < n; ++i) a[i] = i;
    }
    return a;
}

int main(void) {
    {
        int cap = 8;
        int *a = make_seq(cap);
        int ok = a != NULL && trim_to(&a, &cap, 4) == 0;
        ok = ok && cap == 4 && a != NULL;
        if (ok) {
            for (int i = 0; i < 4; ++i) {
                if (a[i] != i) ok = 0;
            }
        }
        report("shrink-keeps-prefix", ok);
        free(a);
    }
    {
        int cap = 5;
        int *a = make_seq(cap);
        if (a != NULL) {
            for (int i = 0; i < cap; ++i) a[i] = 10 + i;
        }
        int ok = a != NULL && trim_to(&a, &cap, 1) == 0;
        ok = ok && cap == 1 && a != NULL && a[0] == 10;
        report("shrink-to-one", ok);
        free(a);
    }
    {
        int cap = 6;
        int *a = make_seq(cap);
        int ok = a != NULL && trim_to(&a, &cap, 0) == 0 && a == NULL && cap == 0;
        ok = ok && trim_to(&a, &cap, 0) == 0 && a == NULL && cap == 0;
        free(a);
        report("free-all", ok);
    }
    {
        int cap = 4;
        int *a = make_seq(cap);
        int *before = a;
        int ok = a != NULL;
        ok = ok && trim_to(&a, &cap, 5) == -1 && a == before && cap == 4;
        ok = ok && trim_to(&a, &cap, -1) == -1 && a == before && cap == 4;
        if (ok) {
            for (int i = 0; i < 4; ++i) {
                if (a[i] != i) ok = 0;
            }
        }
        report("reject-out-of-range", ok);
        free(a);
    }
    {
        int cap = 10;
        int *a = make_seq(cap);
        int ok = a != NULL && trim_to(&a, &cap, 6) == 0 && cap == 6;
        if (ok) {
            for (int i = 0; i < 6; ++i) {
                if (a[i] != i) ok = 0;
            }
        }
        ok = ok && trim_to(&a, &cap, 6) == 0 && cap == 6;
        ok = ok && trim_to(&a, &cap, 3) == 0 && cap == 3;
        if (ok) {
            for (int i = 0; i < 3; ++i) {
                if (a[i] != i) ok = 0;
            }
        }
        report("repeated-shrink", ok);
        free(a);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
