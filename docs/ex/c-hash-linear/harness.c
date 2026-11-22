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
        HashSet s;
        hash_init(&s);
        report("empty-table", hash_size(&s) == 0 && hash_get(&s, 3) == 0 &&
                                  hash_remove(&s, 3) == -1 && hash_size(&s) == 0);
    }
    {
        HashSet s;
        hash_init(&s);
        int ok = hash_set(&s, 1) == 0 && hash_set(&s, 17) == 0 && hash_set(&s, -15) == 0;
        ok = ok && hash_get(&s, 1) == 1 && hash_get(&s, 17) == 1 && hash_get(&s, -15) == 1;
        ok = ok && hash_size(&s) == 3;
        ok = ok && hash_get(&s, 2) == 0 && hash_get(&s, 33) == 0;
        report("collision-chain", ok);
    }
    {
        HashSet s;
        hash_init(&s);
        hash_set(&s, 1);
        hash_set(&s, 17);
        hash_set(&s, 33);
        int ok = hash_remove(&s, 17) == 0;
        ok = ok && hash_get(&s, 1) == 1 && hash_get(&s, 33) == 1;
        ok = ok && hash_get(&s, 17) == 0 && hash_size(&s) == 2;
        ok = ok && hash_remove(&s, 17) == -1;
        report("remove-middle", ok);
    }
    {
        HashSet s;
        hash_init(&s);
        hash_set(&s, 1);
        hash_set(&s, 17);
        hash_set(&s, 33);
        hash_remove(&s, 17);
        int ok = hash_set(&s, 49) == 0;
        ok = ok && hash_get(&s, 49) == 1 && hash_get(&s, 1) == 1 && hash_get(&s, 33) == 1;
        ok = ok && hash_get(&s, 17) == 0 && hash_size(&s) == 3;
        ok = ok && hash_set(&s, 1) == 0 && hash_size(&s) == 3;
        report("reuse-dead-slot", ok);
    }
    {
        HashSet s;
        hash_init(&s);
        int ok = 1;
        for (int k = 0; k < HASH_CAP; ++k) {
            if (hash_set(&s, k) != 0) ok = 0;
        }
        ok = ok && hash_size(&s) == HASH_CAP && hash_get(&s, 7) == 1;
        ok = ok && hash_set(&s, 16) == -1 && hash_size(&s) == HASH_CAP;
        ok = ok && hash_set(&s, 7) == 0 && hash_size(&s) == HASH_CAP;
        ok = ok && hash_remove(&s, 7) == 0 && hash_get(&s, 7) == 0 && hash_size(&s) == 15;
        ok = ok && hash_set(&s, 16) == 0 && hash_get(&s, 16) == 1 && hash_size(&s) == 16;
        report("fill-and-full", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
