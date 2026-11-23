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
        int v = -1;
        int ok = record_reading(0, 25, &v) == ERR_OK && v == 25;
        ok = ok && record_reading(3, READING_MAX, &v) == ERR_OK && v == READING_MAX;
        ok = ok && record_reading(2, 0, &v) == ERR_OK && v == 0;
        report("ok-writes", ok);
    }
    {
        int ok = record_reading(0, 5, NULL) == ERR_ARG_NULL;
        ok = ok && record_reading(-1, 5, NULL) == ERR_ARG_NULL;
        ok = ok && ERR_ARG_NULL == -1 && ERR_ARG_RANGE == -2 && ERR_OK == 0;
        report("null-out", ok);
    }
    {
        int v = 77;
        int ok = record_reading(-1, 5, &v) == ERR_ARG_RANGE && v == 77;
        ok = ok && record_reading(SENSOR_COUNT, 5, &v) == ERR_ARG_RANGE && v == 77;
        ok = ok && record_reading(100, 0, &v) == ERR_ARG_RANGE && v == 77;
        ok = ok && record_reading(3, 5, &v) == ERR_OK && v == 5;
        report("sensor-range", ok);
    }
    {
        int v = 77;
        int ok = record_reading(0, -1, &v) == ERR_ARG_RANGE && v == 77;
        ok = ok && record_reading(1, READING_MAX + 1, &v) == ERR_ARG_RANGE && v == 77;
        ok = ok && record_reading(1, 100000, &v) == ERR_ARG_RANGE && v == 77;
        ok = ok && record_reading(2, 1000, &v) == ERR_OK && v == 1000;
        report("value-range", ok);
    }
    {
        int v = 555;
        int ok = record_reading(0, -1, &v) == ERR_ARG_RANGE && v == 555;
        ok = ok && record_reading(9, 9, &v) == ERR_ARG_RANGE && v == 555;
        ok = ok && record_reading(0, 9, NULL) == ERR_ARG_NULL && v == 555;
        ok = ok && record_reading(1, 321, &v) == ERR_OK && v == 321;
        report("unchanged-on-failure", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
