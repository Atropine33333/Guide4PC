// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

static int on_grid(const void *p) {
    return ((uintptr_t)p % 8u) == 0u;
}

int main(void) {
    {
        Arena a;
        arena_init(&a);
        int ok = arena_used(&a) == 0;
        unsigned char *p = (unsigned char *)arena_alloc(&a, 16);
        ok = ok && p == a.buf && on_grid(p) && arena_used(&a) == 16;
        if (p != NULL) memset(p, 0x31, 16);
        unsigned char *q = (unsigned char *)arena_alloc(&a, 16);
        ok = ok && q == a.buf + 16 && on_grid(q) && arena_used(&a) == 32;
        ok = ok && p != NULL && p[0] == 0x31 && p[15] == 0x31;
        report("fresh-camp", ok);
    }
    {
        Arena a;
        arena_init(&a);
        unsigned char *p = (unsigned char *)arena_alloc(&a, 40);
        int ok = p == a.buf && on_grid(p) && arena_used(&a) == 40;
        if (p != NULL) memset(p, 'A', 40);
        arena_reset(&a);
        ok = ok && arena_used(&a) == 0;
        unsigned char *q = (unsigned char *)arena_alloc(&a, 40);
        ok = ok && q == p && on_grid(q) && arena_used(&a) == 40;
        if (q != NULL) {
            memset(q, 'B', 40);
            ok = ok && q[0] == 'B' && q[39] == 'B';
        } else {
            ok = 0;
        }
        report("reset-reuse", ok);
    }
    {
        Arena a;
        arena_init(&a);
        int ok = arena_alloc(&a, ARENA_CAP + 1) == NULL && arena_used(&a) == 0;
        void *p = arena_alloc(&a, ARENA_CAP);
        ok = ok && p == (void *)a.buf && arena_used(&a) == ARENA_CAP;
        ok = ok && arena_alloc(&a, 1) == NULL && arena_used(&a) == ARENA_CAP;
        report("reject-too-big", ok);
    }
    {
        Arena a;
        arena_init(&a);
        unsigned char *p = (unsigned char *)arena_alloc(&a, 16);
        int ok = p != NULL;
        if (p != NULL) memset(p, 0x77, 16);
        void *big = arena_alloc(&a, ARENA_CAP - 16 + 1);
        ok = ok && big == NULL && arena_used(&a) == 16;
        ok = ok && p != NULL && p[0] == 0x77 && p[15] == 0x77;
        void *rest = arena_alloc(&a, ARENA_CAP - 16);
        ok = ok && rest == (void *)(a.buf + 16) && arena_used(&a) == ARENA_CAP;
        report("reject-keeps-data", ok);
    }
    {
        Arena a;
        arena_init(&a);
        int ok = arena_alloc(&a, 0) == NULL && arena_used(&a) == 0;
        arena_reset(&a);
        ok = ok && arena_used(&a) == 0;
        unsigned char *p = (unsigned char *)arena_alloc(&a, 1);
        ok = ok && p == a.buf && arena_used(&a) == 8;
        arena_reset(&a);
        unsigned char *q = (unsigned char *)arena_alloc(&a, 1);
        ok = ok && q == p && arena_used(&a) == 8;
        report("zero-and-reuse", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
