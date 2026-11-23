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
    {
        int a[6] = {3, 1, 4, 1, 5, 999999};
        report("sum-regular", sum_array(a, 5) == 14);
    }
    {
        int a[2] = {777, 888};
        report("sum-empty", sum_array(a, 0) == 0);
    }
    {
        int a[5] = {-8, 5, -2, 10, -999999};
        report("sum-negative", sum_array(a, 4) == 5);
    }
    {
        int a[5] = {2, 7, 1, 9, -1000000};
        int b[2] = {42, -1000000};
        report("last-element", last_value(a, 4) == 9 && last_value(b, 1) == 42);
    }
    {
        int a[101];
        for (int i = 0; i < 100; ++i) {
            a[i] = i;
        }
        a[100] = -1000000;
        report("long-run", sum_array(a, 100) == 4950 && last_value(a, 100) == 99);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
