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
        Cfg c;
        c.mode = 7u;
        c.level = 15u;
        c.enabled = 1u;
        cfg_init(&c);
        int ok = cfg_mode(&c) == 0u && cfg_level(&c) == 0u && cfg_enabled(&c) == 0;
        report("init-clears", ok);
    }
    {
        Cfg c;
        cfg_init(&c);
        int ok = cfg_set_mode(&c, 7u) == 0 && cfg_mode(&c) == CFG_MODE_MAX;
        ok = ok && cfg_set_mode(&c, 8u) == -1 && cfg_mode(&c) == 7u;
        ok = ok && cfg_set_mode(&c, 0u) == 0 && cfg_mode(&c) == 0u;
        report("mode-limits", ok);
    }
    {
        Cfg c;
        cfg_init(&c);
        int ok = cfg_set_level(&c, 15u) == 0 && cfg_level(&c) == CFG_LEVEL_MAX;
        ok = ok && cfg_set_level(&c, 16u) == -1 && cfg_level(&c) == 15u;
        ok = ok && cfg_set_level(&c, 3u) == 0 && cfg_level(&c) == 3u;
        report("level-limits", ok);
    }
    {
        Cfg c;
        cfg_init(&c);
        int ok = cfg_set_enabled(&c, 1) == 0 && cfg_enabled(&c) == 1;
        ok = ok && cfg_set_enabled(&c, 0) == 0 && cfg_enabled(&c) == 0;
        ok = ok && cfg_set_enabled(&c, 2) == -1 && cfg_enabled(&c) == 0;
        ok = ok && cfg_set_enabled(&c, -3) == -1 && cfg_enabled(&c) == 0;
        report("enabled-toggle", ok);
    }
    {
        Cfg c;
        cfg_init(&c);
        int ok = cfg_set_level(&c, 12u) == 0;
        ok = ok && cfg_set_enabled(&c, 1) == 0;
        ok = ok && cfg_set_mode(&c, 5u) == 0;
        ok = ok && cfg_mode(&c) == 5u && cfg_level(&c) == 12u && cfg_enabled(&c) == 1;
        ok = ok && cfg_set_level(&c, 16u) == -1;
        ok = ok && cfg_mode(&c) == 5u && cfg_level(&c) == 12u && cfg_enabled(&c) == 1;
        ok = ok && cfg_set_mode(&c, 8u) == -1;
        ok = ok && cfg_mode(&c) == 5u && cfg_level(&c) == 12u && cfg_enabled(&c) == 1;
        report("neighbours-kept", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
