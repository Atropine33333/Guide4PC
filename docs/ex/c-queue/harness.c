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
        IntQueue q;
        int v = 42;
        queue_init(&q);
        report("empty-state", queue_size(&q) == 0 && queue_is_empty(&q) == 1 &&
                                queue_is_full(&q) == 0 && queue_pop(&q, &v) == -1 && v == 42);
    }
    {
        IntQueue q;
        int v = 0;
        queue_init(&q);
        int ok = 1;
        for (int i = 1; i <= 4; ++i) {
            if (queue_push(&q, i) != 0) ok = 0;
        }
        for (int i = 1; i <= 4; ++i) {
            if (queue_pop(&q, &v) != 0 || v != i) ok = 0;
        }
        ok = ok && queue_is_empty(&q) == 1;
        report("fifo-order", ok);
    }
    {
        IntQueue q;
        int v = 0;
        queue_init(&q);
        int ok = 1;
        for (int i = 1; i <= QUEUE_CAP; ++i) {
            if (queue_push(&q, i) != 0) ok = 0;
        }
        for (int i = 1; i <= 5; ++i) {
            if (queue_pop(&q, &v) != 0 || v != i) ok = 0;
        }
        ok = ok && queue_push(&q, 9) == 0 && queue_push(&q, 10) == 0 &&
             queue_push(&q, 11) == 0 && queue_size(&q) == 6;
        for (int i = 6; i <= 11; ++i) {
            if (queue_pop(&q, &v) != 0 || v != i) ok = 0;
        }
        ok = ok && queue_pop(&q, &v) == -1 && v == 11;
        report("wrap-around", ok);
    }
    {
        IntQueue q;
        int v = 0;
        queue_init(&q);
        int ok = 1;
        for (int i = 0; i < QUEUE_CAP; ++i) {
            if (queue_push(&q, i * 3) != 0) ok = 0;
        }
        ok = ok && queue_is_full(&q) == 1 && queue_size(&q) == QUEUE_CAP;
        ok = ok && queue_push(&q, 99) == -1 && queue_size(&q) == QUEUE_CAP;
        ok = ok && queue_pop(&q, &v) == 0 && v == 0;
        ok = ok && queue_push(&q, 99) == 0 && queue_is_full(&q) == 1;
        ok = ok && queue_pop(&q, &v) == 0 && v == 3;
        report("full-rejects", ok);
    }
    {
        IntQueue q;
        int v = 0;
        queue_init(&q);
        int ok = 1;
        for (int round = 0; round < 100; ++round) {
            if (queue_push(&q, round) != 0) ok = 0;
            if (queue_pop(&q, &v) != 0 || v != round) ok = 0;
            if (queue_size(&q) != 0) ok = 0;
        }
        ok = ok && queue_is_empty(&q) == 1 && queue_pop(&q, &v) == -1;
        report("drain-and-reuse", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
