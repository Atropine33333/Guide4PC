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
        UnionFind u;
        uf_init(&u, 5);
        int ok = uf_groups(&u) == 5;
        for (int i = 0; i < 5; ++i) {
            if (uf_find(&u, i) != i) ok = 0;
            if (!uf_connected(&u, i, i)) ok = 0;
        }
        ok = ok && !uf_connected(&u, 0, 1);
        ok = ok && uf_find(&u, -1) == -1 && uf_find(&u, 5) == -1;
        report("init-alone", ok);
    }
    {
        UnionFind u;
        uf_init(&u, 6);
        int ok = uf_union(&u, 3, 1) == 1;
        ok = ok && uf_find(&u, 3) == 1 && uf_find(&u, 1) == 1;
        ok = ok && uf_groups(&u) == 5;
        ok = ok && uf_union(&u, 1, 3) == 0;
        ok = ok && uf_groups(&u) == 5;
        report("merge-roots", ok);
    }
    {
        UnionFind u;
        uf_init(&u, 6);
        int ok = uf_union(&u, 1, 0) == 1 && uf_union(&u, 2, 1) == 1 &&
                 uf_union(&u, 3, 2) == 1;
        ok = ok && uf_find(&u, 3) == 0 && uf_find(&u, 2) == 0;
        ok = ok && uf_connected(&u, 0, 3) && !uf_connected(&u, 0, 4);
        ok = ok && uf_groups(&u) == 3;
        report("transitive", ok);
    }
    {
        UnionFind u;
        uf_init(&u, 6);
        int ok = uf_union(&u, 5, 2) == 1 && uf_find(&u, 5) == 2;
        ok = ok && uf_union(&u, 4, 3) == 1 && uf_find(&u, 4) == 3;
        ok = ok && uf_union(&u, 3, 2) == 1;
        ok = ok && uf_find(&u, 4) == 2 && uf_find(&u, 5) == 2;
        ok = ok && uf_connected(&u, 4, 5) && uf_groups(&u) == 3;
        report("smallest-root", ok);
    }
    {
        UnionFind u;
        uf_init(&u, UF_CAP);
        int ok = 1;
        for (int i = 1; i < UF_CAP; ++i) {
            if (uf_union(&u, i, i - 1) != 1) ok = 0;
        }
        ok = ok && uf_groups(&u) == 1 && uf_find(&u, UF_CAP - 1) == 0;
        ok = ok && uf_union(&u, -1, 0) == -1 && uf_union(&u, 0, UF_CAP) == -1;
        ok = ok && uf_connected(&u, -1, 0) == 0 && uf_groups(&u) == 1;
        report("full-range", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
