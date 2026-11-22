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
        IntList l;
        int v = 77;
        list_init(&l);
        report("empty-behaviour", list_size(&l) == 0 && list_is_empty(&l) == 1 &&
                                     list_pop_front(&l, &v) == -1 && v == 77);
    }
    {
        IntList l;
        int v = 0;
        list_init(&l);
        int ok = list_push_front(&l, 1) == 0 && list_push_front(&l, 2) == 0 &&
                 list_push_front(&l, 3) == 0;
        ok = ok && list_pop_front(&l, &v) == 0 && v == 3;
        ok = ok && list_pop_front(&l, &v) == 0 && v == 2;
        ok = ok && list_size(&l) == 1 && list_pop_front(&l, &v) == 0 && v == 1;
        ok = ok && list_is_empty(&l) == 1;
        list_free(&l);
        report("push-front-order", ok);
    }
    {
        IntList l;
        list_init(&l);
        int ok = 1;
        for (int i = 0; i < 500; ++i) {
            if (list_push_front(&l, i) != 0) ok = 0;
        }
        size_t walked = 0;
        for (const Node *n = l.head; n != NULL; n = n->next) ++walked;
        ok = ok && list_size(&l) == 500 && walked == 500 && list_is_empty(&l) == 0;
        list_free(&l);
        report("size-tracking", ok);
    }
    {
        IntList l;
        int v = 0;
        list_init(&l);
        list_push_front(&l, 5);
        list_push_front(&l, 6);
        int ok = list_pop_front(&l, &v) == 0 && v == 6;
        ok = ok && list_pop_front(&l, &v) == 0 && v == 5;
        ok = ok && list_is_empty(&l) == 1 && list_size(&l) == 0;
        ok = ok && list_pop_front(&l, &v) == -1 && v == 5;
        list_free(&l);
        report("pop-to-empty", ok);
    }
    {
        IntList l;
        int v = 0;
        list_init(&l);
        list_push_front(&l, 4);
        list_free(&l);
        int ok = list_size(&l) == 0 && list_is_empty(&l) == 1 && l.head == NULL;
        ok = ok && list_push_front(&l, 9) == 0 && list_pop_front(&l, &v) == 0 && v == 9;
        ok = ok && list_is_empty(&l) == 1;
        list_free(&l);
        report("reuse-after-free", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
