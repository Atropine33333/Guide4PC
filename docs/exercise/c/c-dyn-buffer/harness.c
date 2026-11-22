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
        Buf b;
        buf_init(&b);
        int ok = b.p == NULL && b.len == 0 && b.cap == 0;
        buf_free(&b);
        ok = ok && b.p == NULL && b.len == 0 && b.cap == 0;
        buf_free(&b);
        report("init-and-free", ok);
    }
    {
        Buf b;
        char big[301];
        for (int i = 0; i < 300; ++i) big[i] = (char)('a' + i % 26);
        big[300] = '\0';
        buf_init(&b);
        int ok = buf_append(&b, "guide") == 0 && b.len == 5 && b.cap >= b.len + 1 &&
                 b.p != NULL && strcmp(b.p, "guide") == 0;
        buf_free(&b);
        buf_init(&b);
        ok = ok && buf_append(&b, big) == 0 && b.len == 300 && b.cap >= b.len + 1 &&
             strcmp(b.p, big) == 0;
        buf_free(&b);
        report("single-append", ok);
    }
    {
        Buf b;
        char want[1001];
        char chunk[16];
        buf_init(&b);
        for (int i = 0; i < 100; ++i) {
            memcpy(want + i * 10, "0123456789", 10);
            memcpy(chunk, "0123456789", 11);
            if (buf_append(&b, chunk) != 0) {
                buf_free(&b);
                report("repeated-appends", 0);
                printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
                return 0;
            }
        }
        want[1000] = '\0';
        int ok = b.len == 1000 && b.cap >= b.len + 1 && b.p != NULL && strcmp(b.p, want) == 0;
        buf_free(&b);
        report("repeated-appends", ok);
    }
    {
        Buf b;
        buf_init(&b);
        int ok = buf_append(&b, "") == 0 && b.len == 0 && (b.p == NULL || b.p[0] == '\0');
        ok = ok && buf_append(&b, "tail") == 0 && b.len == 4 && strcmp(b.p, "tail") == 0;
        buf_free(&b);
        report("empty-append", ok);
    }
    {
        Buf b;
        buf_init(&b);
        int ok = buf_append(&b, "first") == 0;
        buf_free(&b);
        buf_init(&b);
        ok = ok && buf_append(&b, "second") == 0 && b.len == 6 && strcmp(b.p, "second") == 0;
        buf_free(&b);
        report("reuse-after-free", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
