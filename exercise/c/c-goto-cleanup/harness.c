// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        char *p = dup_n(NULL, 4);
        report("null-source", p == NULL);
    }
    {
        char *a = dup_n("abc", -1);
        char *b = dup_n(NULL, -1);
        report("negative-count", a == NULL && b == NULL);
    }
    {
        const char *src = "hello";
        char *p = dup_n(src, 5);
        int ok = p != NULL && p != src && strlen(p) == 5 && strcmp(p, "hello") == 0;
        free(p);
        report("exact-copy", ok);
    }
    {
        char *p = dup_n("hello", 3);
        int ok = p != NULL && strlen(p) == 3 && strcmp(p, "hel") == 0;
        free(p);
        report("truncates", ok);
    }
    {
        char *p = dup_n("ab", 5);
        int ok = p != NULL && strcmp(p, "ab") == 0 && p[2] == '\0';
        free(p);
        char *e = dup_n("", 0);
        ok = ok && e != NULL && e[0] == '\0';
        free(e);
        report("short-source", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
