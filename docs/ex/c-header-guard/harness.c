// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>

#ifdef G4_CHANNEL_H
#define G4_HAS_IFACE_GUARD 1
#else
#define G4_HAS_IFACE_GUARD 0
#endif

#ifdef G4_CHANNEL_IMPL_H
#define G4_HAS_IMPL_GUARD 1
#else
#define G4_HAS_IMPL_GUARD 0
#endif

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        int ok = G4_HAS_IFACE_GUARD == 1;
        ok = ok && CHANNEL_MAX == 4 && CHANNEL_MAGIC == 0x5Au;
        ok = ok && channel_magic() == CHANNEL_MAGIC;
        report("interface-guard", ok);
    }
    {
        report("impl-guard", G4_HAS_IMPL_GUARD == 1);
    }
    {
        Channel ch;
        ch.id = -1;
        ch.gain = -1;
        ch.open = -1;
        int ok = channel_open(&ch, 2, 40) == 0;
        ok = ok && channel_is_open(&ch) == 1;
        ok = ok && ch.id == 2 && ch.gain == 40;
        ok = ok && channel_close(&ch) == 0;
        ok = ok && channel_is_open(&ch) == 0;
        ok = ok && channel_close(&ch) == -1;
        report("open-close", ok);
    }
    {
        Channel ch;
        int ok = channel_open(&ch, 0, 0) == 0;
        ok = ok && channel_set_gain(&ch, 100) == 0 && ch.gain == 100;
        ok = ok && channel_set_gain(&ch, 101) == -1 && ch.gain == 100;
        ok = ok && channel_set_gain(&ch, -1) == -1 && ch.gain == 100;
        ok = ok && channel_close(&ch) == 0;
        ok = ok && channel_set_gain(&ch, 50) == -1;
        report("gain-limits", ok);
    }
    {
        Channel ch;
        ch.id = 7;
        ch.gain = 7;
        ch.open = 0;
        int ok = channel_open(0, 0, 0) == -1;
        ok = ok && channel_open(&ch, -1, 0) == -1 && ch.open == 0;
        ok = ok && channel_open(&ch, CHANNEL_MAX, 0) == -1 && ch.open == 0;
        ok = ok && channel_open(&ch, 3, 10) == 0;
        ok = ok && ch.id == 3 && ch.gain == 10;
        ok = ok && channel_is_open(0) == 0;
        report("id-and-null", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
