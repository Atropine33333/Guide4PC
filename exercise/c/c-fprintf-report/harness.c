// Guide4PC harness appended after the student's code in the same translation unit.
#include <limits.h>
#include <stdio.h>
#include <string.h>

static int g_total = 0;
static int g_failed = 0;

static void note(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

/* 调用一次 report，读回首行：要求行末带换行、内容与 want 逐字相同、后面没有多余内容。 */
static int writes_one_line(int id, int score, const char *want) {
    FILE *f = tmpfile();
    if (f == NULL) return 0;

    report(f, id, score);
    rewind(f);

    char buf[256] = {0};
    char extra[8] = {0};
    int ok = fgets(buf, sizeof buf, f) != NULL;
    if (ok) {
        size_t n = strlen(buf);
        int has_nl = n > 0 && buf[n - 1] == '\n';
        while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r')) buf[--n] = '\0';
        ok = has_nl && strcmp(buf, want) == 0;
    }
    if (ok) ok = fgets(extra, sizeof extra, f) == NULL;

    fclose(f);
    return ok;
}

int main(void) {
    note("basic-line", writes_one_line(7, 93, "id=7 score=93"));
    note("zero-values", writes_one_line(0, 0, "id=0 score=0"));
    note("negative-values", writes_one_line(-12, -5, "id=-12 score=-5"));

    {
        char want[64];
        snprintf(want, sizeof want, "id=%d score=%d", INT_MAX, INT_MIN);
        note("extreme-int", writes_one_line(INT_MAX, INT_MIN, want));
    }

    {
        FILE *f = tmpfile();
        int ok = f != NULL;
        if (ok) {
            report(f, 1, 2);
            report(f, 3, 4);
            rewind(f);

            char a[128] = {0};
            char b[128] = {0};
            char extra[8] = {0};
            ok = fgets(a, sizeof a, f) != NULL && fgets(b, sizeof b, f) != NULL;
            if (ok) {
                size_t na = strlen(a);
                size_t nb = strlen(b);
                int nla = na > 0 && a[na - 1] == '\n';
                int nlb = nb > 0 && b[nb - 1] == '\n';
                while (na > 0 && (a[na - 1] == '\n' || a[na - 1] == '\r')) a[--na] = '\0';
                while (nb > 0 && (b[nb - 1] == '\n' || b[nb - 1] == '\r')) b[--nb] = '\0';
                ok = nla && nlb && strcmp(a, "id=1 score=2") == 0 && strcmp(b, "id=3 score=4") == 0;
            }
            if (ok) ok = fgets(extra, sizeof extra, f) == NULL;

            fclose(f);
        }
        note("two-lines", ok);
    }

    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
