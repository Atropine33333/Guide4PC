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
        RingBuf r;
        int v = 123;
        ring_init(&r);
        int ok = ring_size(&r) == 0;
        ok = ok && ring_pop(&r, &v) == -1 && v == 123;
        ok = ok && ring_peek(&r, &v) == -1 && v == 123;
        report("empty", ok);
    }
    {
        RingBuf r;
        int v = 0;
        ring_init(&r);
        int ok = ring_push(&r, 1) == 0 && ring_push(&r, 2) == 0 && ring_push(&r, 3) == 0;
        ok = ok && ring_size(&r) == 3;
        ok = ok && ring_peek(&r, &v) == 0 && v == 1 && ring_size(&r) == 3;
        ok = ok && ring_pop(&r, &v) == 0 && v == 1;
        ok = ok && ring_pop(&r, &v) == 0 && v == 2;
        ok = ok && ring_pop(&r, &v) == 0 && v == 3;
        ok = ok && ring_size(&r) == 0;
        ok = ok && ring_pop(&r, &v) == -1;
        report("fifo", ok);
    }
    {
        RingBuf r;
        int v = 0;
        ring_init(&r);
        int ok = 1;
        for (int i = 1; i <= RING_CAP; ++i) {
            if (ring_push(&r, i) != 0) ok = 0;
        }
        for (int i = 1; i <= 3; ++i) {
            if (ring_pop(&r, &v) != 0 || v != i) ok = 0;
        }
        ok = ok && ring_push(&r, 9) == 0;
        ok = ok && ring_push(&r, 10) == 0;
        ok = ok && ring_push(&r, 11) == 0;
        ok = ok && ring_size(&r) == RING_CAP;
        for (int i = 4; i <= RING_CAP; ++i) {
            if (ring_pop(&r, &v) != 0 || v != i) ok = 0;
        }
        for (int i = 9; i <= 11; ++i) {
            if (ring_pop(&r, &v) != 0 || v != i) ok = 0;
        }
        ok = ok && ring_size(&r) == 0;
        ok = ok && ring_pop(&r, &v) == -1;
        report("wrap-around", ok);
    }
    {
        RingBuf r;
        int v = 0;
        ring_init(&r);
        int ok = 1;
        for (int i = 1; i <= RING_CAP; ++i) {
            if (ring_push(&r, i * 10) != 0) ok = 0;
        }
        ok = ok && ring_size(&r) == RING_CAP;
        ok = ok && ring_push(&r, 99) == -1;
        ok = ok && ring_size(&r) == RING_CAP;
        ok = ok && ring_peek(&r, &v) == 0 && v == 10;
        for (int i = 1; i <= RING_CAP; ++i) {
            if (ring_pop(&r, &v) != 0 || v != i * 10) ok = 0;
        }
        ok = ok && ring_pop(&r, &v) == -1;
        report("full", ok);
    }
    {
        RingBuf r;
        int v = 0;
        ring_init(&r);
        int ok = 1;
        for (int i = 1; i <= RING_CAP; ++i) {
            if (ring_push(&r, i * 10) != 0) ok = 0;
        }
        for (int i = 1; i <= 5; ++i) {
            if (ring_pop(&r, &v) != 0 || v != i * 10) ok = 0;
        }
        ok = ok && ring_size(&r) == 3;
        for (int i = 9; i <= 13; ++i) {
            if (ring_push(&r, i * 10) != 0) ok = 0;
        }
        ok = ok && ring_size(&r) == RING_CAP;
        for (int i = 6; i <= 13; ++i) {
            if (ring_pop(&r, &v) != 0 || v != i * 10) ok = 0;
        }
        ok = ok && ring_pop(&r, &v) == -1;
        report("wrap-after-pop", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
