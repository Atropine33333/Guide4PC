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

int main(void) {
    {
        int *a = NULL;
        int n = 0;
        int cap = 0;
        int ok = append(&a, &n, &cap, 42) == 0;
        ok = ok && n == 1 && cap == 4 && a != NULL && a[0] == 42;
        report("first-append", ok);
        free(a);
    }
    {
        int *a = NULL;
        int n = 0;
        int cap = 0;
        int ok = 1;
        for (int i = 0; i < 10; ++i) {
            if (append(&a, &n, &cap, i * 3) != 0) ok = 0;
        }
        ok = ok && n == 10 && cap == 16 && a != NULL;
        if (a != NULL) {
            for (int i = 0; i < 10; ++i) {
                if (a[i] != i * 3) ok = 0;
            }
        }
        report("keeps-old-values", ok);
        free(a);
    }
    {
        int *a = NULL;
        int n = 0;
        int cap = 0;
        int ok = 1;
        for (int i = 1; i <= 5; ++i) {
            if (append(&a, &n, &cap, i) != 0) ok = 0;
            if (i <= 4 && cap != 4) ok = 0;
            if (i == 5 && cap != 8) ok = 0;
        }
        ok = ok && n == 5 && a != NULL;
        if (a != NULL) {
            for (int i = 0; i < 5; ++i) {
                if (a[i] != i + 1) ok = 0;
            }
        }
        report("grows-capacity", ok);
        free(a);
    }
    {
        int *a = malloc(2 * sizeof(int));
        int n = 0;
        int cap = 2;
        int ok = a != NULL;
        if (a != NULL) {
            a[0] = 10;
            a[1] = 20;
            n = 2;
            ok = ok && append(&a, &n, &cap, 30) == 0;
            ok = ok && n == 3 && cap == 4 && a != NULL;
            ok = ok && a[0] == 10 && a[1] == 20 && a[2] == 30;
        }
        report("existing-buffer", ok);
        free(a);
    }
    {
        int *a = NULL;
        int n = 0;
        int cap = 0;
        int ok = 1;
        for (int i = 0; i < 1000; ++i) {
            if (append(&a, &n, &cap, i - 500) != 0) ok = 0;
        }
        ok = ok && n == 1000 && cap == 1024 && a != NULL;
        if (a != NULL) {
            for (int i = 0; i < 1000; ++i) {
                if (a[i] != i - 500) ok = 0;
            }
        }
        report("long-run", ok);
        free(a);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
