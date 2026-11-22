// 追加在学生代码之后的检查程序，与 solution.c 编在同一个翻译单元里。
#include <stdio.h>
#include <string.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

static void fill(char *p, size_t n, char c) {
    for (size_t i = 0; i < n; ++i) p[i] = c;
}

int main(void) {
    {
        char dst[16];
        fill(dst, sizeof dst, 'X');
        copy_name(dst, 8, "0123456");
        report("exact-fit", strcmp(dst, "0123456") == 0 && dst[8] == 'X');
    }
    {
        char dst[16];
        fill(dst, sizeof dst, 'X');
        copy_name(dst, 5, "abcdefgh");
        report("truncated", strcmp(dst, "abcd") == 0 && dst[5] == 'X');
    }
    {
        char dst[4];
        fill(dst, sizeof dst, 'Q');
        copy_name(dst, 0, "zz");
        report("zero-cap", memcmp(dst, "QQQQ", 4) == 0);
    }
    {
        char dst[8];
        fill(dst, sizeof dst, 'Y');
        copy_name(dst, 8, "");
        report("empty-source", dst[0] == '\0' && dst[1] == 'Y');
    }
    {
        char dst[8];
        fill(dst, sizeof dst, 'Y');
        copy_name(dst, 1, "abc");
        report("cap-one", dst[0] == '\0' && dst[1] == 'Y');
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
