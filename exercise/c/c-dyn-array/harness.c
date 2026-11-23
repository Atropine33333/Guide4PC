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
        IntVec v;
        vec_init(&v);
        int ok = v.size == 0 && v.cap == 0 && v.data == NULL;
        vec_free(&v);
        report("empty-vector", ok && v.size == 0);
    }
    {
        IntVec v;
        vec_init(&v);
        int ok = vec_push(&v, 7) == 0 && vec_push(&v, 9) == 0 && vec_push(&v, -4) == 0;
        ok = ok && v.size == 3 && v.cap >= 3 && v.data != NULL;
        if (ok) ok = v.data[0] == 7 && v.data[1] == 9 && v.data[2] == -4;
        vec_free(&v);
        report("push-basic", ok);
    }
    {
        IntVec v;
        vec_init(&v);
        int ok = 1;
        for (int i = 0; i < 500; ++i) {
            if (vec_push(&v, i * 3 - 50) != 0) {
                ok = 0;
                break;
            }
        }
        if (ok) ok = v.size == 500 && v.cap >= 500 && v.data != NULL;
        for (int i = 0; ok && i < 500; ++i) {
            if (v.data[i] != i * 3 - 50) ok = 0;
        }
        vec_free(&v);
        report("growth-values", ok);
    }
    {
        IntVec v;
        vec_init(&v);
        int ok = 1;
        for (int i = 0; i < 300; ++i) {
            size_t before = v.cap;
            if (vec_push(&v, i) != 0) {
                ok = 0;
                break;
            }
            if (v.cap < v.size || v.cap < before) ok = 0;
            if (v.cap > before) {
                size_t doubled = before == 0 ? 1 : before * 2;
                if (v.cap < doubled) ok = 0;
            }
        }
        vec_free(&v);
        report("capacity-doubling", ok);
    }
    {
        IntVec v;
        vec_init(&v);
        int pushed = vec_push(&v, 10) == 0 && vec_push(&v, 20) == 0;
        vec_free(&v);
        report("free-resets", pushed && v.size == 0);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
