// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
#include <limits.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        SortedArray a;
        sa_init(&a);
        int ok = sa_size(&a) == 0;
        ok = ok && sa_insert(&a, 5) == 0 && sa_size(&a) == 1 && a.data[0] == 5;
        report("empty-and-single", ok);
    }
    {
        SortedArray a;
        sa_init(&a);
        int vals[5] = { 5, 1, 4, 2, 3 };
        int ok = 1;
        for (int i = 0; i < 5; ++i) {
            if (sa_insert(&a, vals[i]) != 0) ok = 0;
        }
        ok = ok && sa_size(&a) == 5;
        for (int i = 0; i < 5; ++i) {
            if (a.data[i] != i + 1) ok = 0;
        }
        report("keeps-order", ok);
    }
    {
        SortedArray a;
        sa_init(&a);
        int vals[5] = { 2, 2, 1, 2, 1 };
        int ok = 1;
        for (int i = 0; i < 5; ++i) {
            if (sa_insert(&a, vals[i]) != 0) ok = 0;
        }
        int expect[5] = { 1, 1, 2, 2, 2 };
        ok = ok && sa_size(&a) == 5;
        for (int i = 0; i < 5; ++i) {
            if (a.data[i] != expect[i]) ok = 0;
        }
        report("duplicates", ok);
    }
    {
        SortedArray a;
        sa_init(&a);
        int ok = 1;
        for (int i = 0; i < ARR_CAP; ++i) {
            if (sa_insert(&a, i) != 0) ok = 0;
        }
        ok = ok && sa_size(&a) == ARR_CAP;
        ok = ok && sa_insert(&a, -1) == -1 && sa_size(&a) == ARR_CAP;
        ok = ok && a.data[0] == 0 && a.data[ARR_CAP - 1] == ARR_CAP - 1;
        report("full", ok);
    }
    {
        SortedArray a;
        sa_init(&a);
        int ok = sa_insert(&a, 0) == 0;
        ok = ok && sa_insert(&a, INT_MAX) == 0;
        ok = ok && sa_insert(&a, INT_MIN) == 0;
        ok = ok && sa_insert(&a, -7) == 0;
        ok = ok && sa_size(&a) == 4;
        ok = ok && a.data[0] == INT_MIN && a.data[1] == -7 && a.data[2] == 0 &&
                  a.data[3] == INT_MAX;
        report("negatives", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
