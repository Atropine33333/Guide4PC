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
        Pool p;
        p.in_use = 77;
        for (int i = 0; i < POOL_BLOCK_COUNT; ++i) p.used[i] = 7;
        pool_init(&p);
        int ok = p.in_use == 0 && pool_in_use(&p) == 0;
        for (int i = 0; ok && i < POOL_BLOCK_COUNT; ++i) {
            if (p.used[i] != 0) ok = 0;
        }
        void *first = pool_alloc(&p);
        ok = ok && first == (void *)p.cells[0] && p.in_use == 1 && pool_in_use(&p) == 1;
        report("fresh-pool", ok);
    }
    {
        Pool p;
        pool_init(&p);
        void *blocks[POOL_BLOCK_COUNT];
        int ok = 1;
        for (int i = 0; i < POOL_BLOCK_COUNT; ++i) {
            blocks[i] = pool_alloc(&p);
            if (blocks[i] != (void *)p.cells[i]) ok = 0;
        }
        ok = ok && p.in_use == POOL_BLOCK_COUNT;
        ok = ok && pool_alloc(&p) == NULL && p.in_use == POOL_BLOCK_COUNT;
        report("alloc-until-full", ok);
    }
    {
        Pool p;
        pool_init(&p);
        unsigned char *blocks[POOL_BLOCK_COUNT];
        int ok = 1;
        for (int i = 0; i < POOL_BLOCK_COUNT; ++i) {
            blocks[i] = (unsigned char *)pool_alloc(&p);
            if (blocks[i] == NULL) ok = 0;
        }
        for (int i = 0; ok && i < POOL_BLOCK_COUNT; ++i) {
            for (int b = 0; b < POOL_BLOCK_SIZE; ++b) blocks[i][b] = (unsigned char)(0x10 + i);
        }
        for (int i = 0; ok && i < POOL_BLOCK_COUNT; ++i) {
            for (int b = 0; b < POOL_BLOCK_SIZE; ++b) {
                if (blocks[i][b] != (unsigned char)(0x10 + i)) ok = 0;
            }
        }
        report("cell-isolation", ok);
    }
    {
        Pool p;
        pool_init(&p);
        for (int i = 0; i < POOL_BLOCK_COUNT; ++i) pool_alloc(&p);
        int ok = pool_free(&p, p.cells[3]) == 0 && p.in_use == POOL_BLOCK_COUNT - 1;
        ok = ok && pool_free(&p, p.cells[3]) == -1 && p.in_use == POOL_BLOCK_COUNT - 1;
        int foreign = 0;
        ok = ok && pool_free(&p, &foreign) == -1;
        ok = ok && pool_free(&p, NULL) == -1;
        ok = ok && pool_free(&p, (void *)((unsigned char *)p.cells[3] + 1)) == -1;
        int probes = 0;
        for (int i = 0; i < 3; ++i) {
            void *again = pool_alloc(&p);
            if (again == (void *)p.cells[3]) ++probes;
        }
        ok = ok && probes == 1 && p.in_use == POOL_BLOCK_COUNT;
        report("reuse-and-validate", ok);
    }
    {
        Pool p;
        pool_init(&p);
        int ok = 1;
        for (int round = 0; round < 2; ++round) {
            for (int i = 0; i < 4; ++i) {
                if (pool_alloc(&p) == NULL) ok = 0;
            }
            pool_reset(&p);
            if (p.in_use != 0 || pool_in_use(&p) != 0) ok = 0;
            for (int i = 0; i < POOL_BLOCK_COUNT; ++i) {
                if (p.used[i] != 0) ok = 0;
            }
        }
        void *again = pool_alloc(&p);
        ok = ok && again == (void *)p.cells[0] && p.in_use == 1;
        pool_reset(NULL);
        report("reset-refills", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
