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
        char buf[16] = "abc";
        int ok = append_str(buf, sizeof buf, "def") == 0;
        ok = ok && strcmp(buf, "abcdef") == 0;
        report("simple-append", ok);
    }
    {
        char a[7] = "abc";
        char b[8] = "abc";
        int ok = append_str(a, sizeof a, "def") == 0;
        ok = ok && strcmp(a, "abcdef") == 0;
        ok = ok && append_str(b, sizeof b, "def") == 0;
        ok = ok && append_str(b, sizeof b, "g") == 0;
        ok = ok && strcmp(b, "abcdefg") == 0;
        report("exact-fit", ok);
    }
    {
        char buf[8] = "abc";
        int ok = append_str(buf, sizeof buf, "defgh") == -1;
        ok = ok && strcmp(buf, "abc") == 0;
        ok = ok && append_str(buf, sizeof buf, "de") == 0;
        ok = ok && strcmp(buf, "abcde") == 0;
        report("too-long", ok);
    }
    {
        char buf[8] = "abc";
        int ok = append_str(buf, sizeof buf, "") == 0;
        ok = ok && strcmp(buf, "abc") == 0;
        report("empty-string", ok);
    }
    {
        char buf[8] = "";
        char tiny[1] = "";
        int ok = append_str(buf, sizeof buf, "xy") == 0;
        ok = ok && strcmp(buf, "xy") == 0;
        ok = ok && append_str(tiny, sizeof tiny, "") == 0;
        ok = ok && append_str(tiny, sizeof tiny, "x") == -1;
        ok = ok && tiny[0] == '\0';
        report("empty-buffer", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
