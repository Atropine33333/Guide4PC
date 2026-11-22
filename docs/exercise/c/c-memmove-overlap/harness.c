// 追加在学生代码之后的检查程序，与 solution.c 编在同一个翻译单元里。
#include <stdio.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

static int same(const int *a, const int *want, int n) {
    for (int i = 0; i < n; ++i) {
        if (a[i] != want[i]) return 0;
    }
    return 1;
}

int main(void) {
    {
        int a[8] = {1, 2, 3, 4, 5, 6, -1, -1};
        const int want[6] = {3, 4, 5, 6, 0, 0};
        shift_left(a, 6, 2);
        report("overlap-shift", same(a, want, 6) && a[6] == -1 && a[7] == -1);
    }
    {
        int a[4] = {10, 20, 30, 40};
        const int want[4] = {20, 30, 40, 0};
        shift_left(a, 4, 1);
        report("shift-one", same(a, want, 4));
    }
    {
        int a[3] = {7, 8, 9};
        const int want[3] = {7, 8, 9};
        shift_left(a, 3, 0);
        report("no-shift", same(a, want, 3));
    }
    {
        int a[4] = {1, 2, 3, 4};
        shift_left(a, 4, 4);
        report("shift-everything", a[0] == 0 && a[1] == 0 && a[2] == 0 && a[3] == 0);
    }
    {
        int a[1] = {5};
        shift_left(a, 1, 1);
        report("single-element", a[0] == 0);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
