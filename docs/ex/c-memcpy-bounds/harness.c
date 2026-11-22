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
        char dst[16];
        memset(dst, 0x5A, sizeof dst);
        size_t r = copy_capped(dst, sizeof dst, "hello", 5);
        int ok = r == 5 && strcmp(dst, "hello") == 0 && dst[5] == '\0';
        ok = ok && (unsigned char)dst[6] == 0x5A;
        report("fits-whole", ok);
    }
    {
        char dst[4];
        memset(dst, 0x5A, sizeof dst);
        size_t r = copy_capped(dst, 4, "abcdef", 6);
        int ok = r == 3 && dst[0] == 'a' && dst[1] == 'b' && dst[2] == 'c' && dst[3] == '\0';
        report("cap-truncates", ok);
    }
    {
        char dst[8];
        memset(dst, 0x5A, sizeof dst);
        size_t r = copy_capped(dst, sizeof dst, "abcdef", 2);
        int ok = r == 2 && dst[0] == 'a' && dst[1] == 'b' && dst[2] == '\0';
        ok = ok && (unsigned char)dst[5] == 0x5A;
        report("n-smaller", ok);
    }
    {
        char src[64];
        char dst[8];
        memset(src, 'x', sizeof src);
        memset(dst, 0x5A, sizeof dst);
        size_t r = copy_capped(dst, 5, src, 64);
        int ok = r == 4;
        for (int i = 0; i < 4; ++i) {
            if (dst[i] != 'x') ok = 0;
        }
        ok = ok && dst[4] == '\0';
        for (int i = 5; i < 8; ++i) {
            if ((unsigned char)dst[i] != 0x5A) ok = 0;
        }
        report("no-overrun", ok);
    }
    {
        char dst[4] = {'z', 'z', 'z', 'z'};
        size_t r0 = copy_capped(dst, 0, "abc", 3);
        int ok = r0 == 0 && dst[0] == 'z' && dst[1] == 'z' && dst[2] == 'z' && dst[3] == 'z';
        size_t r1 = copy_capped(dst, 1, "abc", 3);
        ok = ok && r1 == 0 && dst[0] == '\0' && dst[1] == 'z';
        report("cap-one-and-zero", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
