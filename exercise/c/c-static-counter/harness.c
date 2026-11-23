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
        int ok = counter_get() == 0;
        ok = ok && counter_next() == 1;
        ok = ok && counter_get() == 1;
        ok = ok && counter_next() == 2;
        ok = ok && counter_get() == 2;
        report("starts-at-zero", ok);
    }
    {
        counter_reset();
        int ok = counter_add(10) == 10;
        ok = ok && counter_add(-3) == 7;
        ok = ok && counter_get() == 7;
        ok = ok && counter_add(0) == 7;
        ok = ok && counter_get() == 7;
        report("add-and-read", ok);
    }
    {
        counter_reset();
        counter_next();
        counter_next();
        counter_next();
        counter_reset();
        int ok = counter_get() == 0;
        ok = ok && counter_next() == 1;
        ok = ok && counter_add(-5) == -4;
        counter_reset();
        ok = ok && counter_get() == 0;
        report("reset-starts-over", ok);
    }
    {
        counter_reset();
        int ok = 1;
        for (int i = 1; i <= 100; ++i) {
            if (counter_next() != i) ok = 0;
        }
        ok = ok && counter_get() == 100;
        ok = ok && counter_add(23) == 123;
        ok = ok && counter_next() == 124;
        report("long-run", ok);
    }
    {
        counter_reset();
        int ok = counter_add(7) == 7;
        ok = ok && counter_get() == 7;
        ok = ok && counter_next() == 8;
        ok = ok && counter_get() == 8;
        counter_reset();
        ok = ok && counter_get() == 0;
        ok = ok && counter_add(2) == 2;
        report("mixed-calls", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
