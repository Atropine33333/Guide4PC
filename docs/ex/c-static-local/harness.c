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
        reset_ids();
        int ok = next_id() == 1 && next_id() == 2 && next_id() == 3;
        report("first-sequence", ok);
    }
    {
        int ok = next_id() == 4 && next_id() == 5;
        report("keeps-counting", ok);
    }
    {
        reset_ids();
        int ok = next_id() == 1 && next_id() == 2;
        report("reset-restarts", ok);
    }
    {
        int ok = 0;
        reset_ids();
        reset_ids();
        ok = next_id() == 1;
        reset_ids();
        ok = ok && next_id() == 1 && next_id() == 2;
        report("reset-twice", ok);
    }
    {
        reset_ids();
        int ok = 1;
        for (int i = 1; i <= 100; ++i) {
            if (next_id() != i) {
                ok = 0;
                break;
            }
        }
        report("long-run", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
