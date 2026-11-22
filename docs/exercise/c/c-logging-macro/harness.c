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
        int n = LOG(buf, sizeof buf, 1, "start %d", 7);
        int ok = n == 14 && strcmp(buf, "[INFO] start 7") == 0;
        report("basic-line", ok);
    }
    {
        char b[32];
        int ok = 1;
        LOG(b, sizeof b, 0, "a");
        ok = ok && strcmp(b, "[DEBUG] a") == 0;
        LOG(b, sizeof b, 2, "b");
        ok = ok && strcmp(b, "[WARN] b") == 0;
        LOG(b, sizeof b, 3, "c");
        ok = ok && strcmp(b, "[ERROR] c") == 0;
        LOG(b, sizeof b, 7, "d");
        ok = ok && strcmp(b, "[LVL7] d") == 0;
        report("level-names", ok);
    }
    {
        char b[64];
        int n = LOG(b, sizeof b, 2, "%s=%d", "x", 42);
        int ok = n == 11 && strcmp(b, "[WARN] x=42") == 0;
        n = LOG(b, sizeof b, 0, "%03d|%.2f", 5, 1.5);
        ok = ok && n == 16 && strcmp(b, "[DEBUG] 005|1.50") == 0;
        report("format-args", ok);
    }
    {
        char small[10];
        int n = LOG(small, sizeof small, 1, "%s", "abcdefghij");
        int ok = n == 17 && strlen(small) == 9 && strcmp(small, "[INFO] ab") == 0;
        char tiny[4];
        n = LOG(tiny, sizeof tiny, 3, "zz");
        ok = ok && n == 10 && strlen(tiny) == 3 && strcmp(tiny, "[ER") == 0;
        report("truncate", ok);
    }
    {
        char b[8];
        int ok = LOG(NULL, 8, 1, "x") == -1;
        ok = ok && LOG(b, 0, 1, "x") == -1;
        ok = ok && LOG(b, sizeof b, 1, NULL) == -1;
        report("bad-args", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
