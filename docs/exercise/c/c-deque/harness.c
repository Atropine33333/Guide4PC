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
        Deque d;
        int v = -7;
        deque_init(&d);
        int ok = deque_size(&d) == 0;
        ok = ok && deque_pop_front(&d, &v) == -1 && v == -7;
        ok = ok && deque_pop_back(&d, &v) == -1 && v == -7;
        ok = ok && deque_size(&d) == 0;
        report("empty-behaviour", ok);
    }
    {
        Deque d;
        int v = 0;
        deque_init(&d);
        int ok = deque_push_back(&d, 1) == 0 && deque_push_back(&d, 2) == 0 &&
                 deque_push_front(&d, 0) == 0;
        ok = ok && deque_size(&d) == 3;
        ok = ok && deque_pop_front(&d, &v) == 0 && v == 0;
        ok = ok && deque_pop_front(&d, &v) == 0 && v == 1;
        ok = ok && deque_pop_front(&d, &v) == 0 && v == 2;
        ok = ok && deque_size(&d) == 0;
        report("front-order", ok);
    }
    {
        Deque d;
        int v = 0;
        deque_init(&d);
        for (int i = 1; i <= 4; ++i) {
            deque_push_back(&d, i);
        }
        int ok = deque_size(&d) == 4;
        ok = ok && deque_pop_back(&d, &v) == 0 && v == 4;
        ok = ok && deque_pop_back(&d, &v) == 0 && v == 3;
        ok = ok && deque_pop_front(&d, &v) == 0 && v == 1;
        ok = ok && deque_pop_back(&d, &v) == 0 && v == 2;
        ok = ok && deque_size(&d) == 0;
        report("back-order", ok);
    }
    {
        Deque d;
        int v = 0;
        deque_init(&d);
        int ok = 1;
        for (int i = 0; i < DEQUE_CAP; ++i) {
            if (deque_push_back(&d, i) != 0) ok = 0;
        }
        ok = ok && deque_size(&d) == DEQUE_CAP;
        for (int i = 0; i < 10; ++i) {
            if (deque_pop_front(&d, &v) != 0 || v != i) ok = 0;
        }
        for (int i = DEQUE_CAP; i < DEQUE_CAP + 10; ++i) {
            if (deque_push_back(&d, i) != 0) ok = 0;
        }
        ok = ok && deque_size(&d) == DEQUE_CAP;
        for (int i = 10; i < DEQUE_CAP + 10; ++i) {
            if (deque_pop_front(&d, &v) != 0 || v != i) ok = 0;
        }
        ok = ok && deque_size(&d) == 0;
        report("wrap-around", ok);
    }
    {
        Deque d;
        int v = 0;
        deque_init(&d);
        int ok = 1;
        for (int i = 0; i < DEQUE_CAP; ++i) {
            if (deque_push_front(&d, i) != 0) ok = 0;
        }
        ok = ok && deque_size(&d) == DEQUE_CAP;
        ok = ok && deque_push_front(&d, -1) == -1 && deque_push_back(&d, -1) == -1;
        ok = ok && deque_size(&d) == DEQUE_CAP;
        for (int i = 0; i < DEQUE_CAP; ++i) {
            if (deque_pop_back(&d, &v) != 0 || v != i) ok = 0;
        }
        ok = ok && deque_size(&d) == 0;
        report("full-rejection", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
