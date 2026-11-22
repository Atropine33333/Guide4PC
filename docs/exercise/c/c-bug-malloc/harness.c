// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
#include <stdlib.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        int *p = make_squares(6);
        int ok = p != NULL && live_blocks() == 1;
        if (p != NULL) {
            for (int i = 0; i < 6; ++i) {
                if (p[i] != i * i) ok = 0;
            }
        }
        release_squares(p);
        ok = ok && live_blocks() == 0;
        report("values", ok);
    }
    {
        release_squares(NULL);
        release_squares(NULL);
        report("null-release", live_blocks() == 0);
    }
    {
        int *a = make_squares(3);
        int *b = make_squares(4);
        int ok = a != NULL && b != NULL && live_blocks() == 2;
        if (a != NULL) {
            ok = ok && a[2] == 4;
            release_squares(a);
        }
        ok = ok && live_blocks() == 1;
        if (b != NULL) {
            ok = ok && b[3] == 9;
            release_squares(b);
        }
        ok = ok && live_blocks() == 0;
        report("two-blocks", ok);
    }
    {
        long before = live_blocks();
        int ok = make_squares(0) == NULL;
        ok = ok && make_squares(-1) == NULL;
        ok = ok && make_squares(SQ_MAX + 1) == NULL;
        ok = ok && live_blocks() == before;
        report("invalid-size", ok);
    }
    {
        int ok = 1;
        for (int round = 0; round < 200; ++round) {
            int *p = make_squares(8);
            if (p == NULL || p[7] != 49) {
                ok = 0;
                break;
            }
            release_squares(p);
        }
        ok = ok && live_blocks() == 0;
        report("reuse-loop", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
