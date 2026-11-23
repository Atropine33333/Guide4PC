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
        int x = 7;
        int y = -3;
        swap_ints(&x, &y);
        report("swap-basic", x == -3 && y == 7);
    }
    {
        int arr[5] = {10, 20, 30, 40, 50};
        swap_ints(&arr[1], &arr[3]);
        report("swap-array", arr[0] == 10 && arr[1] == 40 && arr[2] == 30 &&
                                 arr[3] == 20 && arr[4] == 50);
    }
    {
        int arr[6] = {3, 9, 4, 9, 1, 9};
        int *p = max_ptr(arr, 6);
        report("max-pointer", p == &arr[1] && *p == 9);
    }
    {
        int arr[4] = {42, 5, -6, 1};
        int *p = max_ptr(arr, 4);
        report("max-first", p == &arr[0] && *p == 42);
    }
    {
        int value = 5;
        int *p = max_ptr(&value, 1);
        swap_ints(&value, &value);
        report("max-single", p == &value && value == 5);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
