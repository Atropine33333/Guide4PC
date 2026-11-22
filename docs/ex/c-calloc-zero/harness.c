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
        int *p = zeros(8);
        int ok = p != NULL;
        if (p != NULL) {
            for (int i = 0; i < 8; ++i) {
                if (p[i] != 0) ok = 0;
            }
        }
        report("zero-filled", ok);
        free(p);
    }
    {
        int *p = zeros(5);
        int ok = p != NULL;
        if (p != NULL) {
            for (int i = 0; i < 5; ++i) p[i] = i * 7 - 3;
            for (int i = 0; i < 5; ++i) {
                if (p[i] != i * 7 - 3) ok = 0;
            }
        }
        report("writable", ok);
        free(p);
    }
    {
        int *p = zeros(0);
        int ok = p == NULL;
        free(p);
        free(NULL);
        report("zero-count", ok);
    }
    {
        int *p = zeros(4096);
        int ok = p != NULL;
        if (p != NULL) {
            ok = ok && p[0] == 0 && p[2048] == 0 && p[4095] == 0;
        }
        report("large-block", ok);
        free(p);
    }
    {
        int *a = zeros(3);
        int *b = zeros(3);
        int ok = a != NULL && b != NULL && a != b;
        if (ok) {
            for (int i = 0; i < 3; ++i) {
                if (a[i] != 0 || b[i] != 0) ok = 0;
            }
            a[0] = 11;
            ok = ok && a[0] == 11 && b[0] == 0;
        }
        report("separate-blocks", ok);
        free(a);
        free(b);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
