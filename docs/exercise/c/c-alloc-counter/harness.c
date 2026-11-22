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
        AllocStats st;
        st.allocs = 9;
        st.frees = 9;
        ac_init(&st);
        int ok = st.allocs == 0 && st.frees == 0 && ac_outstanding(&st) == 0;
        report("fresh-zero", ok);
    }
    {
        AllocStats st;
        ac_init(&st);
        void *a = ac_alloc(&st, 16);
        void *b = ac_alloc(&st, 32);
        void *c = ac_alloc(&st, 8);
        int ok = a != NULL && b != NULL && c != NULL;
        ok = ok && a != b && b != c && a != c;
        ok = ok && st.allocs == 3 && st.frees == 0 && ac_outstanding(&st) == 3;
        ac_free(&st, a);
        ac_free(&st, c);
        ok = ok && st.allocs == 3 && st.frees == 2 && ac_outstanding(&st) == 1;
        ac_free(&st, b);
        ok = ok && st.frees == 3 && ac_outstanding(&st) == 0;
        report("counts", ok);
    }
    {
        AllocStats st;
        ac_init(&st);
        void *p = ac_alloc(&st, 12);
        ac_free(&st, NULL);
        int ok = p != NULL && st.allocs == 1 && st.frees == 0 && ac_outstanding(&st) == 1;
        ac_free(&st, p);
        report("null-free-ignored", ok);
    }
    {
        AllocStats st;
        ac_init(&st);
        int *nums = (int *)ac_alloc(&st, 4 * sizeof(int));
        int ok = nums != NULL && st.allocs == 1;
        if (ok) {
            for (int i = 0; i < 4; ++i) nums[i] = i * 11 - 7;
            ok = nums[0] == -7 && nums[1] == 4 && nums[2] == 15 && nums[3] == 26;
        }
        ac_free(&st, nums);
        ok = ok && st.frees == 1 && ac_outstanding(&st) == 0;
        report("memory-usable", ok);
    }
    {
        AllocStats st;
        ac_init(&st);
        void *zero = ac_alloc(&st, 0);
        void *via_null = ac_alloc(NULL, 4);
        int ok = zero == NULL && via_null == NULL;
        ok = ok && st.allocs == 0 && st.frees == 0 && ac_outstanding(&st) == 0;
        void *p = ac_alloc(&st, 4);
        ok = ok && p != NULL && st.allocs == 1;
        ac_free(&st, p);
        report("failure-not-counted", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
