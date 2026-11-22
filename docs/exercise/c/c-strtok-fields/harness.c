// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
#include <string.h>

static int g4pc_total = 0;
static int g4pc_failed = 0;

static void g4pc_report(const char *id, int ok) {
    ++g4pc_total;
    if (!ok) ++g4pc_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

static void g4pc_clear(char out[][FIELD_LEN]) {
    memset(out, 0, FIELD_MAX * FIELD_LEN);
}

int main(void) {
    {
        char out[FIELD_MAX][FIELD_LEN];
        g4pc_clear(out);
        int n = split_fields("  alpha , beta gamma ,, gamma  ", out, FIELD_MAX);
        int ok = n == 3;
        ok = ok && strcmp(out[0], "alpha") == 0;
        ok = ok && strcmp(out[1], "beta gamma") == 0;
        ok = ok && strcmp(out[2], "gamma") == 0;
        ok = ok && out[3][0] == '\0';
        g4pc_report("normal", ok);
    }
    {
        char out[FIELD_MAX][FIELD_LEN];
        g4pc_clear(out);
        int n = split_fields("a,,  ,b, c", out, FIELD_MAX);
        int ok = n == 3;
        ok = ok && strcmp(out[0], "a") == 0;
        ok = ok && strcmp(out[1], "b") == 0;
        ok = ok && strcmp(out[2], "c") == 0;
        ok = ok && split_fields("lead,,trail,", out, FIELD_MAX) == 2;
        ok = ok && strcmp(out[1], "trail") == 0;
        g4pc_report("consecutive-commas", ok);
    }
    {
        char out[FIELD_MAX][FIELD_LEN];
        g4pc_clear(out);
        out[0][0] = 'Q';
        int ok = split_fields("", out, FIELD_MAX) == 0;
        ok = ok && split_fields("   ", out, FIELD_MAX) == 0;
        ok = ok && split_fields(" , , ", out, FIELD_MAX) == 0;
        ok = ok && split_fields(",,,,", out, FIELD_MAX) == 0;
        ok = ok && out[0][0] == 'Q';
        g4pc_report("all-empty", ok);
    }
    {
        char out[FIELD_MAX][FIELD_LEN];
        char line[512];
        int ok = 1;

        size_t pos = 0;
        for (int i = 0; i < 90; ++i) line[pos++] = 'x';
        line[pos++] = ',';
        line[pos++] = 't';
        line[pos++] = 'a';
        line[pos++] = 'i';
        line[pos++] = 'l';
        line[pos] = '\0';

        g4pc_clear(out);
        int n = split_fields(line, out, FIELD_MAX);
        ok = ok && n == 2;
        ok = ok && strlen(out[0]) == FIELD_LEN - 1;
        for (size_t i = 0; ok && i < FIELD_LEN - 1; ++i) {
            if (out[0][i] != 'x') ok = 0;
        }
        ok = ok && strcmp(out[1], "tail") == 0;

        pos = 0;
        for (int i = 0; i < 40; ++i) {
            int written = snprintf(line + pos, sizeof line - pos, i == 0 ? "f%d" : ",f%d", i);
            if (written < 0) {
                ok = 0;
                break;
            }
            pos += (size_t)written;
        }
        g4pc_clear(out);
        out[4][0] = 'Z';
        n = split_fields(line, out, 4);
        ok = ok && n == 4;
        ok = ok && strcmp(out[0], "f0") == 0;
        ok = ok && strcmp(out[1], "f1") == 0;
        ok = ok && strcmp(out[2], "f2") == 0;
        ok = ok && strcmp(out[3], "f3") == 0;
        ok = ok && out[4][0] == 'Z';

        g4pc_clear(out);
        out[0][0] = 'Q';
        ok = ok && split_fields("a,b", out, 0) == 0;
        ok = ok && out[0][0] == 'Q';

        g4pc_report("overlong-line", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g4pc_total, g4pc_failed);
    return 0;
}
