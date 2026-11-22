// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
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
        char *empty = dup_str("");
        int ok = dup_str(NULL) == NULL;
        ok = ok && empty != NULL && empty[0] == '\0';
        free_str(empty);
        report("null-and-empty", ok);
    }
    {
        const char *src = "hello";
        char *copy = dup_str(src);
        int ok = copy != NULL && copy != src && strcmp(copy, src) == 0 && strlen(copy) == 5;
        free_str(copy);
        report("content-copy", ok);
    }
    {
        char src[] = "harbor";
        char *copy = dup_str(src);
        int ok = copy != NULL && strcmp(src, "harbor") == 0;
        if (ok) copy[0] = 'H';
        ok = ok && strcmp(src, "harbor") == 0 && strcmp(copy, "Harbor") == 0;
        free_str(copy);
        report("independent-copy", ok);
    }
    {
        char src[301];
        for (int i = 0; i < 300; ++i) src[i] = (char)('a' + (i % 26));
        src[300] = '\0';
        char *copy = dup_str(src);
        int ok = copy != NULL && strlen(copy) == 300 && strcmp(copy, src) == 0;
        if (ok) ok = copy[299] == src[299] && copy[299] != '\0';
        free_str(copy);
        report("long-copy", ok);
    }
    {
        char *p = dup_str("first");
        int ok = p != NULL;
        free_str(NULL);
        free_str(p);
        char *q = dup_str("second");
        ok = ok && q != NULL && strcmp(q, "second") == 0;
        free_str(q);
        report("free-safe", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
