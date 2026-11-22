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
    int a = 0;
    int b = 0;

    a = -1;
    b = -1;
    int ok = read_pair("12 34", &a, &b) == 2 && a == 12 && b == 34;
    a = 0;
    b = 0;
    ok = ok && read_pair("-3 -4", &a, &b) == 2 && a == -3 && b == -4;
    report("both-values", ok);

    a = 99;
    b = 99;
    ok = read_pair("42", &a, &b) == 1 && a == 42 && b == 99;
    a = 99;
    b = 99;
    ok = ok && read_pair("12 xyz", &a, &b) == 1 && a == 12 && b == 99;
    report("one-value", ok);

    a = 7;
    b = 7;
    ok = read_pair("abc", &a, &b) == 0 && a == 7 && b == 7;
    a = 7;
    b = 7;
    ok = ok && read_pair("x9", &a, &b) == 0 && a == 7 && b == 7;
    report("no-values", ok);

    a = 0;
    b = 0;
    ok = read_pair("5 6 7", &a, &b) == 2 && a == 5 && b == 6;
    report("extra-tokens", ok);

    a = 3;
    b = 4;
    ok = read_pair(NULL, &a, &b) == 0 && a == 3 && b == 4;
    ok = ok && read_pair("", &a, &b) == 0 && a == 3 && b == 4;
    ok = ok && read_pair("   ", &a, &b) == 0 && a == 3 && b == 4;
    report("empty-input", ok);

    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
