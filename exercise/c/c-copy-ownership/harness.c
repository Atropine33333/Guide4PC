// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>
#include <stdlib.h>
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
        Player *p = player_new("Robin", 42);
        Player *q = (p != NULL) ? player_clone(p) : NULL;
        int ok = p != NULL && q != NULL && p != q;
        ok = ok && p->name != NULL && q->name != NULL && p->name != q->name;
        ok = ok && strcmp(p->name, "Robin") == 0 && strcmp(q->name, "Robin") == 0;
        ok = ok && p->score == 42 && q->score == 42;
        player_free(q);
        player_free(p);
        report("fields-copied", ok);
    }
    {
        Player *p = player_new("Robin", 42);
        Player *q = (p != NULL) ? player_clone(p) : NULL;
        int ok = p != NULL && q != NULL && q->name != NULL && q->name[0] != '\0';
        if (ok) {
            q->name[0] = 'X';
            q->score = 7;
        }
        ok = ok && p->name != NULL && strcmp(p->name, "Robin") == 0 && p->score == 42;
        ok = ok && q->name != NULL && strcmp(q->name, "Xobin") == 0 && q->score == 7;
        player_free(q);
        player_free(p);
        report("edit-isolated", ok);
    }
    {
        char buf[16];
        strcpy(buf, "Ada");
        Player *p = player_new(buf, 1);
        strcpy(buf, "Bob");
        int ok = p != NULL && p->name != NULL && strcmp(p->name, "Ada") == 0 && p->score == 1;
        player_free(p);
        report("source-copied", ok);
    }
    {
        Player *p = player_new("Kit", 3);
        Player *q = (p != NULL) ? player_clone(p) : NULL;
        int ok = p != NULL && q != NULL;
        player_free(p);
        ok = ok && q->name != NULL && strcmp(q->name, "Kit") == 0 && q->score == 3;
        player_free(q);
        report("independent-lifetime", ok);
    }
    {
        Player *empty = player_new("", -3);
        int ok = player_new(NULL, 1) == NULL;
        ok = ok && empty != NULL && empty->name != NULL && empty->name[0] == '\0';
        ok = ok && empty->score == -3;
        ok = ok && player_clone(NULL) == NULL;
        player_free(empty);
        player_free(NULL);
        report("null-handling", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
