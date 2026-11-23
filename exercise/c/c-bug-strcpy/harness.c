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
        char buf[64];
        memset(buf, 'Z', sizeof buf);
        int n = label_make(buf, 6, "ab", "cd");
        int ok = n == 5 && strcmp(buf, "ab#cd") == 0;
        report("exact-fit", ok);
    }
    {
        char buf[64];
        char name[40];
        char code[16];
        char want[64];
        for (int i = 0; i < 30; ++i) name[i] = (char)('A' + (i % 26));
        name[30] = '\0';
        for (int i = 0; i < 10; ++i) code[i] = (char)('0' + (i % 10));
        code[10] = '\0';
        int n = label_make(buf, LABEL_CAP, name, code);
        snprintf(want, sizeof want, "%s#%s", name, code);
        want[LABEL_CAP - 1] = '\0';
        int ok = n == LABEL_CAP - 1 && strcmp(buf, want) == 0;
        report("long-cut", ok);
    }
    {
        char buf[64];
        int n = label_quote(buf, LABEL_CAP, "ok");
        int ok = n == 4 && strcmp(buf, "[ok]") == 0;
        n = label_quote(buf, LABEL_CAP, "");
        ok = ok && n == 2 && strcmp(buf, "[]") == 0;
        report("quote-brackets", ok);
    }
    {
        char buf[64];
        int n = label_quote(buf, 12, "abcdefghijklmnop");
        int ok = n == 11 && strcmp(buf, "[abcdefghij") == 0;
        report("quote-long", ok);
    }
    {
        struct {
            char buf[8];
            char guard[4];
        } cell;
        memset(&cell, '#', sizeof cell);
        int n = label_make(cell.buf, 8, "a-very-long-name", "and-a-long-code");
        int ok = n == 7 && strlen(cell.buf) == 7;
        for (int i = 0; i < 4; ++i) {
            if (cell.guard[i] != '#') ok = 0;
        }
        report("guard-intact", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
