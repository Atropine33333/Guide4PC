// 与学生的代码拼在同一个翻译单元里，直接调用函数做各项检查。
#include <stddef.h>
#include <stdio.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    report("literal", str_len("hello") == 5);

    report("empty", str_len("") == 0);

    report("null", str_len(NULL) == 0);

    {
        char buf[8] = "abc";
        int ok = str_len(buf) == 3;
        ok = ok && buf[0] == 'a' && buf[1] == 'b' && buf[2] == 'c' && buf[3] == '\0';
        report("array", ok);
    }

    {
        char big[65];
        for (int i = 0; i < 64; ++i) {
            big[i] = 'x';
        }
        big[64] = '\0';
        report("long-string", str_len(big) == 64);
    }

    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
