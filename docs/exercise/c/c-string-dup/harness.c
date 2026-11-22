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
        const char *src = "hello";
        char *copy = dup_string(src);
        int ok = copy != NULL && copy != src && strcmp(copy, "hello") == 0;
        if (copy != src) free(copy);
        report("basic-copy", ok);
    }
    {
        const char *src = "";
        char *copy = dup_string(src);
        int ok = copy != NULL && copy != src && copy[0] == '\0';
        if (copy != src) free(copy);
        report("empty-string", ok);
    }
    {
        char big[4097];
        for (int i = 0; i < 4096; ++i) big[i] = (char)('a' + i % 26);
        big[4096] = '\0';
        char *copy = dup_string(big);
        int ok = copy != NULL && copy != big && strlen(copy) == 4096 && strcmp(copy, big) == 0;
        if (copy != big) free(copy);
        report("long-string", ok);
    }
    {
        char buffer[] = "alpha";
        char *copy = dup_string(buffer);
        int ok = copy != NULL && copy != buffer && strcmp(copy, "alpha") == 0;
        if (ok) {
            buffer[0] = 'X';
            ok = strcmp(copy, "alpha") == 0 && strcmp(buffer, "Xlpha") == 0;
        }
        if (copy != buffer) free(copy);
        report("independent-copy", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
