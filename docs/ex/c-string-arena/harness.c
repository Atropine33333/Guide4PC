// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
#include <string.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        Arena a;
        a.base = NULL;
        a.cap = 5;
        a.used = 5;
        int rc = arena_init(&a, 256);
        int ok = rc == 0 && a.base != NULL && a.cap == 256 && a.used == 0;
        ok = ok && arena_used(&a) == 0;
        arena_free(&a);
        report("init-empty", ok);
    }
    {
        Arena a;
        int ok = arena_init(&a, 64) == 0;
        char *first = (char *)arena_alloc(&a, 10);
        ok = ok && first != NULL && first == a.base;
        if (ok) {
            for (int i = 0; i < 10; ++i) first[i] = (char)('a' + i);
        }
        char *second = (char *)arena_alloc(&a, 20);
        ok = ok && second != NULL && second == a.base + 10;
        ok = ok && a.used == 30 && arena_used(&a) == 30;
        ok = ok && strncmp(first, "abcdefghij", 10) == 0;
        arena_reset(&a);
        ok = ok && a.used == 0 && arena_used(&a) == 0;
        char *whole = (char *)arena_alloc(&a, 64);
        ok = ok && whole == a.base && arena_alloc(&a, 1) == NULL;
        arena_free(&a);
        report("carve-and-reset", ok);
    }
    {
        Arena a;
        int ok = arena_init(&a, 64) == 0;
        char *hello = arena_strdup(&a, "hello");
        ok = ok && hello != NULL && strcmp(hello, "hello") == 0;
        ok = ok && hello >= a.base && hello < a.base + a.cap;
        ok = ok && a.used == 6;
        char *world = arena_strdup(&a, "world");
        ok = ok && world != NULL && strcmp(world, "world") == 0;
        ok = ok && world == hello + 6 && a.used == 12;
        ok = ok && strcmp(hello, "hello") == 0;
        arena_free(&a);
        report("strdup-content", ok);
    }
    {
        Arena a;
        int ok = arena_init(&a, 8) == 0;
        ok = ok && arena_strdup(&a, "123456789") == NULL && a.used == 0;
        char *fits = arena_strdup(&a, "1234567");
        ok = ok && fits != NULL && strcmp(fits, "1234567") == 0 && a.used == 8;
        ok = ok && arena_strdup(&a, "") == NULL && a.used == 8;
        ok = ok && arena_strdup(&a, "x") == NULL && a.used == 8;
        arena_free(&a);
        report("no-room", ok);
    }
    {
        Arena a;
        int ok = arena_init(&a, 32) == 0;
        void *p = arena_alloc(&a, 32);
        ok = ok && p != NULL && a.used == 32;
        arena_free(&a);
        ok = ok && a.base == NULL && a.cap == 0 && a.used == 0 && arena_used(&a) == 0;
        arena_free(&a);
        ok = ok && arena_alloc(&a, 4) == NULL;
        ok = ok && arena_init(&a, 16) == 0 && a.cap == 16;
        arena_free(&a);
        report("free-safe", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
