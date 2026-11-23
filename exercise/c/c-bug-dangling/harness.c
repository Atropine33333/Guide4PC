// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
#include <string.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        NamePool p;
        pool_init(&p);
        const char *first = pool_register(&p, "Kepler");
        const char *second = pool_register(&p, "Void");
        int ok = first != NULL && second != NULL;
        ok = ok && strcmp(first, "Kepler") == 0;
        ok = ok && strcmp(second, "Void") == 0;
        report("keeps-copy", ok);
    }
    {
        NamePool p;
        pool_init(&p);
        const char *a = pool_register(&p, "A-01");
        const char *b = pool_register(&p, "B-02");
        int ok = a != NULL && b != NULL;
        ok = ok && a != b;
        ok = ok && strcmp(a, "A-01") == 0;
        ok = ok && strcmp(b, "B-02") == 0;
        report("two-shadows", ok);
    }
    {
        NamePool p;
        pool_init(&p);
        const char *held[POOL_CAP];
        int ok = 1;
        for (int i = 0; i < POOL_CAP; ++i) {
            held[i] = pool_register(&p, "held");
            if (held[i] == NULL) ok = 0;
        }
        pool_release(&p, held[2]);
        pool_release(&p, held[5]);
        const char *x = pool_register(&p, "again-x");
        const char *y = pool_register(&p, "again-y");
        ok = ok && x != NULL && y != NULL;
        ok = ok && strcmp(x, "again-x") == 0;
        ok = ok && strcmp(y, "again-y") == 0;
        ok = ok && pool_register(&p, "extra") == NULL;
        report("release-reuse", ok);
    }
    {
        NamePool p;
        pool_init(&p);
        const char *held[POOL_CAP];
        char label[16];
        int ok = 1;
        for (int i = 0; i < POOL_CAP; ++i) {
            snprintf(label, sizeof label, "name-%d", i);
            held[i] = pool_register(&p, label);
            if (held[i] == NULL) ok = 0;
        }
        ok = ok && pool_register(&p, "extra") == NULL;
        for (int i = 0; i < POOL_CAP; ++i) {
            snprintf(label, sizeof label, "name-%d", i);
            if (held[i] == NULL || strcmp(held[i], label) != 0) ok = 0;
        }
        report("pool-full", ok);
    }
    {
        NamePool p;
        pool_init(&p);
        char longname[64];
        for (int i = 0; i < 40; ++i) {
            longname[i] = (char)('a' + (i % 26));
        }
        longname[40] = '\0';
        const char *cut = pool_register(&p, longname);
        int ok = cut != NULL;
        ok = ok && strlen(cut) == (size_t)(SLOT_LEN - 1);
        ok = ok && strncmp(cut, longname, (size_t)(SLOT_LEN - 1)) == 0;
        const char *tail = pool_register(&p, "tail");
        ok = ok && tail != NULL && strcmp(tail, "tail") == 0;
        report("truncate", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
