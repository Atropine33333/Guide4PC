// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

static int on_grid(const void *p) {
    return ((uintptr_t)p % 64u) == 0u;
}

int main(void) {
    {
        unsigned char *p = (unsigned char *)alloc_aligned64(100);
        int ok = p != NULL && on_grid(p);
        if (ok) {
            memset(p, 0xAB, 100);
            ok = p[0] == 0xAB && p[99] == 0xAB;
        }
        free_aligned64(p);
        report("basic-alignment", ok);
    }
    {
        size_t sizes[] = {1, 7, 63, 64, 65, 127, 128, 1000, 4096};
        int ok = 1;
        for (size_t i = 0; i < sizeof sizes / sizeof sizes[0]; ++i) {
            size_t n = sizes[i];
            unsigned char *b = (unsigned char *)alloc_aligned64(n);
            if (b == NULL || !on_grid(b)) {
                ok = 0;
                free_aligned64(b);
                continue;
            }
            b[0] = 0x11;
            if (n > 1) b[n - 1] = 0x22;
            if (b[0] != 0x11 || (n > 1 && b[n - 1] != 0x22)) ok = 0;
            free_aligned64(b);
        }
        report("size-range", ok);
    }
    {
        int ok = alloc_aligned64(0) == NULL;
        free_aligned64(NULL);
        report("zero-and-null", ok);
    }
    {
        unsigned char *a = (unsigned char *)alloc_aligned64(64);
        unsigned char *b = (unsigned char *)alloc_aligned64(64);
        int ok = a != NULL && b != NULL && a != b && on_grid(a) && on_grid(b);
        if (ok) {
            memset(a, 0x11, 64);
            memset(b, 0x22, 64);
            ok = a[0] == 0x11 && a[63] == 0x11 && b[0] == 0x22 && b[63] == 0x22;
        }
        free_aligned64(a);
        free_aligned64(b);
        report("distinct-blocks", ok);
    }
    {
        int ok = 1;
        for (int i = 0; i < 64; ++i) {
            unsigned char *b = (unsigned char *)alloc_aligned64(32);
            if (b == NULL || !on_grid(b)) {
                ok = 0;
                free_aligned64(b);
                break;
            }
            b[0] = (unsigned char)i;
            b[31] = (unsigned char)(255 - i);
            if (b[0] != (unsigned char)i || b[31] != (unsigned char)(255 - i)) ok = 0;
            free_aligned64(b);
        }
        report("repeat-cycle", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
