// 与学生的代码拼在同一个翻译单元里，直接调用函数做各项检查。
#include <stdio.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    clear_flag();
    report("cleared-returns-zero", flag == 0 && wait_until_set(8) == 0 && flag == 0);

    set_flag();
    report("already-set", flag == 1 && wait_until_set(1) == 1);

    {
        clear_flag();
        int ok = wait_until_set(0) == 0;
        set_flag();
        ok = ok && wait_until_set(0) == 0;
        ok = ok && wait_until_set(1) == 1;
        report("zero-limit", ok);
    }

    {
        clear_flag();
        int ok = flag == 0;
        set_flag();
        ok = ok && flag == 1;
        ok = ok && wait_until_set(3) == 1;
        clear_flag();
        ok = ok && flag == 0;
        ok = ok && wait_until_set(3) == 0;
        report("toggle", ok);
    }

    {
        clear_flag();
        report("long-wait", wait_until_set(1000000) == 0 && flag == 0);
    }

    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
