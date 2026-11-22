// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        IntFlex *v = flex_new(5);
        int ok = v != NULL && v->n == 5;
        for (int i = 0; ok && i < 5; ++i) {
            if (v->a[i] != 0) ok = 0;
        }
        ok = ok && flex_sum(v) == 0;
        flex_free(v);
        report("new-zeros", ok);
    }
    {
        const int src[3] = {3, -1, 7};
        IntFlex *v = flex_from(src, 3);
        int ok = v != NULL && v->n == 3;
        ok = ok && v->a[0] == 3 && v->a[1] == -1 && v->a[2] == 7;
        ok = ok && flex_get(v, 0) == 3 && flex_get(v, 2) == 7;
        ok = ok && flex_sum(v) == 9;
        flex_free(v);
        report("from-values", ok);
    }
    {
        IntFlex *v = flex_new(4);
        flex_set(v, 2, 99);
        int ok = v != NULL && flex_get(v, 2) == 99 && v->a[2] == 99;
        ok = ok && flex_get(v, 0) == 0 && v->n == 4 && flex_sum(v) == 99;
        flex_set(v, 4, 5);
        flex_set(v, 100, 6);
        ok = ok && v->a[2] == 99 && v->n == 4 && flex_sum(v) == 99;
        ok = ok && flex_get(v, 4) == 0 && flex_get(v, 100) == 0;
        flex_free(v);
        report("set-and-get", ok);
    }
    {
        IntFlex *empty = flex_new(0);
        int ok = empty != NULL && empty->n == 0 && flex_sum(empty) == 0;
        flex_free(empty);
        IntFlex *blank = flex_from(NULL, 0);
        ok = ok && blank != NULL && blank->n == 0 && flex_sum(blank) == 0;
        flex_free(blank);
        ok = ok && flex_from(NULL, 2) == NULL;
        report("empty-plot", ok);
    }
    {
        flex_free(NULL);
        const int src[2] = {4, 5};
        IntFlex *v = flex_from(src, 2);
        int ok = v != NULL && flex_sum(v) == 9;
        flex_free(v);
        IntFlex *again = flex_new(3);
        ok = ok && again != NULL && again->n == 3 && flex_sum(again) == 0;
        flex_free(again);
        report("free-safe", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
