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
        Flags f;
        f.a = 1;
        f.b = 3;
        f.c = 31;
        flags_init(&f);
        int ok = flags_get(&f, 'a') == 0 && flags_get(&f, 'b') == 0 && flags_get(&f, 'c') == 0;
        ok = ok && flags_pack(&f) == 0u;
        report("init-clears", ok);
    }
    {
        Flags f;
        flags_init(&f);
        int ok = flags_set(&f, 'a', 1) == 0;
        ok = ok && flags_get(&f, 'a') == 1 && flags_get(&f, 'b') == 0 && flags_get(&f, 'c') == 0;
        ok = ok && flags_pack(&f) == 1u;
        report("single-field", ok);
    }
    {
        Flags f;
        flags_init(&f);
        int ok = flags_set(&f, 'b', 2) == 0 && flags_set(&f, 'c', 31) == 0;
        ok = ok && flags_set(&f, 'a', 1) == 0;
        ok = ok && flags_get(&f, 'b') == 2 && flags_get(&f, 'c') == 31;
        ok = ok && flags_pack(&f) == (unsigned)((1u) | (2u << 1) | (31u << 3));
        report("field-limits", ok);
    }
    {
        Flags f;
        flags_init(&f);
        int ok = flags_set(&f, 'b', 3) == 0;
        unsigned before = flags_pack(&f);
        ok = ok && flags_set(&f, 'b', 4) == -1;
        ok = ok && flags_set(&f, 'c', 32) == -1;
        ok = ok && flags_set(&f, 'a', 2) == -1;
        ok = ok && flags_set(&f, 'x', 0) == -1;
        ok = ok && flags_get(&f, 'x') == 0;
        ok = ok && flags_get(&f, 'b') == 3;
        ok = ok && flags_pack(&f) == before;
        report("range-rejected", ok);
    }
    {
        Flags f;
        flags_init(&f);
        int ok = flags_unpack(&f, 0xA5u) == 0;
        ok = ok && flags_get(&f, 'a') == 1 && flags_get(&f, 'b') == 2 && flags_get(&f, 'c') == 20;
        ok = ok && flags_pack(&f) == 0xA5u;
        unsigned kept = flags_pack(&f);
        ok = ok && flags_unpack(&f, 0x100u) == -1;
        ok = ok && flags_pack(&f) == kept;
        ok = ok && flags_unpack(&f, 0u) == 0;
        ok = ok && flags_pack(&f) == 0u;
        report("unpack-roundtrip", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
