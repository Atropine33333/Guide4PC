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
        IntStack s;
        int v = 123;
        stack_init(&s);
        report("empty-behaviour", stack_size(&s) == 0 && stack_pop(&s, &v) == -1 &&
                                     stack_peek(&s, &v) == -1 && v == 123);
    }
    {
        IntStack s;
        int v = 0;
        stack_init(&s);
        int ok = stack_push(&s, 7) == 0 && stack_push(&s, 9) == 0;
        ok = ok && stack_size(&s) == 2;
        ok = ok && stack_peek(&s, &v) == 0 && v == 9;
        report("push-peek", ok);
    }
    {
        IntStack s;
        int v = 0;
        stack_init(&s);
        stack_push(&s, 1);
        stack_push(&s, 2);
        stack_push(&s, 3);
        int ok = stack_pop(&s, &v) == 0 && v == 3;
        ok = ok && stack_pop(&s, &v) == 0 && v == 2;
        ok = ok && stack_size(&s) == 1 && stack_peek(&s, &v) == 0 && v == 1;
        report("lifo-order", ok);
    }
    {
        IntStack s;
        stack_init(&s);
        int ok = 1;
        for (int i = 0; i < INT_STACK_CAP; ++i) {
            if (stack_push(&s, i) != 0) ok = 0;
        }
        ok = ok && stack_size(&s) == INT_STACK_CAP;
        ok = ok && stack_push(&s, -1) == -1;
        ok = ok && stack_size(&s) == INT_STACK_CAP;
        report("full-stack", ok);
    }
    {
        IntStack s;
        IntStack copy;
        int v = 0;
        stack_init(&s);
        stack_push(&s, 4);
        stack_push(&s, 5);
        copy = s;
        int ok = stack_pop(&s, &v) == 0 && v == 5;
        ok = ok && stack_size(&copy) == 2 && stack_pop(&copy, &v) == 0 && v == 5;
        report("struct-copy", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
