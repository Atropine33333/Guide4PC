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
        BitSet b;
        bitset_init(&b, 10);
        int ok = bitset_count(&b) == 0;
        for (int i = 0; i < 10; ++i) {
            if (bitset_test(&b, i) != 0) ok = 0;
        }
        ok = ok && bitset_test(&b, -1) == -1 && bitset_test(&b, 10) == -1;
        report("init-empty", ok);
    }
    {
        BitSet b;
        bitset_init(&b, 16);
        int ok = bitset_set(&b, 0) == 0 && bitset_set(&b, 3) == 0 && bitset_set(&b, 9) == 0;
        ok = ok && bitset_count(&b) == 3;
        ok = ok && bitset_test(&b, 0) == 1 && bitset_test(&b, 3) == 1 && bitset_test(&b, 9) == 1;
        ok = ok && bitset_test(&b, 1) == 0;
        ok = ok && bitset_set(&b, 3) == 0 && bitset_count(&b) == 3;
        ok = ok && bitset_set(&b, 16) == -1 && bitset_count(&b) == 3;
        report("set-count", ok);
    }
    {
        BitSet b;
        bitset_init(&b, 8);
        for (int i = 0; i < 8; ++i) {
            bitset_set(&b, i);
        }
        int ok = bitset_count(&b) == 8;
        ok = ok && bitset_clear(&b, 0) == 0 && bitset_clear(&b, 4) == 0;
        ok = ok && bitset_count(&b) == 6;
        ok = ok && bitset_clear(&b, 0) == 0 && bitset_count(&b) == 6;
        ok = ok && bitset_clear(&b, 8) == -1 && bitset_count(&b) == 6;
        ok = ok && bitset_test(&b, 4) == 0 && bitset_test(&b, 5) == 1;
        report("clear", ok);
    }
    {
        BitSet b;
        bitset_init(&b, 13);
        int ok = bitset_set(&b, 7) == 0 && bitset_set(&b, 8) == 0;
        ok = ok && bitset_count(&b) == 2;
        ok = ok && bitset_test(&b, 7) == 1 && bitset_test(&b, 8) == 1;
        ok = ok && bitset_set(&b, 12) == 0 && bitset_count(&b) == 3;
        ok = ok && bitset_test(&b, 11) == 0;
        report("byte-boundary", ok);
    }
    {
        BitSet b;
        bitset_init(&b, BITSET_MAX_BITS);
        int ok = bitset_set(&b, 0) == 0 && bitset_set(&b, BITSET_MAX_BITS - 1) == 0;
        ok = ok && bitset_count(&b) == 2;
        ok = ok && bitset_test(&b, BITSET_MAX_BITS - 1) == 1;
        ok = ok && bitset_set(&b, BITSET_MAX_BITS) == -1;
        ok = ok && bitset_count(&b) == 2;
        bitset_init(&b, 0);
        ok = ok && bitset_count(&b) == 0 && bitset_set(&b, 0) == -1;
        report("limits", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
