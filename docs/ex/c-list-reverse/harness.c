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
        Node *r = list_reverse(NULL);
        report("empty-list", r == NULL);
    }
    {
        Node a = { 5, NULL };
        Node *r = list_reverse(&a);
        report("single-node", r == &a && a.value == 5 && a.next == NULL);
    }
    {
        Node a = { 1, NULL };
        Node b = { 2, NULL };
        a.next = &b;
        Node *r = list_reverse(&a);
        report("two-nodes", r == &b && b.next == &a && a.next == NULL);
    }
    {
        Node n[10];
        for (int i = 0; i < 10; ++i) {
            n[i].value = i + 1;
            n[i].next = (i + 1 < 10) ? &n[i + 1] : NULL;
        }
        Node *r = list_reverse(&n[0]);
        int ok = r == &n[9];
        Node *p = r;
        int expect = 10;
        for (int i = 0; i < 10 && p != NULL; ++i) {
            if (p->value != expect) ok = 0;
            --expect;
            p = p->next;
        }
        ok = ok && p == NULL && expect == 0;
        report("many-nodes", ok);
    }
    {
        Node n[8];
        for (int i = 0; i < 8; ++i) {
            n[i].value = i;
            n[i].next = (i + 1 < 8) ? &n[i + 1] : NULL;
        }
        Node *r = list_reverse(&n[0]);
        int ok = r == &n[7];
        Node *p = r;
        for (int i = 0; i < 8 && p != NULL; ++i) {
            if (p != &n[7 - i]) ok = 0;
            p = p->next;
        }
        ok = ok && p == NULL;
        report("nodes-reused", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
