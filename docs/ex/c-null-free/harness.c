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
        int *v = (int *)malloc(sizeof *v);
        int ok = v != NULL;
        if (v != NULL) *v = 5;
        void *slot = v;
        safe_free(&slot);
        ok = ok && slot == NULL;
        report("frees-and-clears", ok);
    }
    {
        int *v = (int *)malloc(sizeof *v);
        int ok = v != NULL;
        if (v != NULL) *v = 9;
        void *slot = v;
        safe_free(&slot);
        safe_free(&slot);
        safe_free(&slot);
        ok = ok && slot == NULL;
        report("double-call-safe", ok);
    }
    {
        safe_free(NULL);
        report("null-pointer", 1);
    }
    {
        void *slot = NULL;
        safe_free(&slot);
        report("null-value", slot == NULL);
    }
    {
        int *x = (int *)malloc(sizeof *x);
        int *y = (int *)malloc(sizeof *y);
        void *sx = x;
        void *sy = y;
        int ok = x != NULL && y != NULL;
        if (x != NULL) *x = 1;
        if (y != NULL) *y = 2;
        safe_free(&sx);
        ok = ok && sx == NULL && sy == y;
        if (y != NULL) *(int *)sy = 7;
        ok = ok && y != NULL && *y == 7;
        safe_free(&sy);
        ok = ok && sy == NULL;
        report("neighbour-slots", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
