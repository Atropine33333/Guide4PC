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
    report("empty", sum_ints(0) == 0);
    report("three", sum_ints(3, 1, 2, 3) == 6);
    report("five", sum_ints(5, 10, 20, 30, 40, 50) == 150);
    report("negatives",
           sum_ints(4, -5, 3, -2, 10) == 6 &&
           sum_ints(2, -7, -8) == -15 &&
           sum_ints(1, 0) == 0);
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
