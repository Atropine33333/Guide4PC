// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>

#define TRACK_MAX 64

static Node g_nodes[TRACK_MAX];
static int g_live[TRACK_MAX];
static int g_alloc_count = 0;
static int g_live_count = 0;
static int g_free_count = 0;
static int g_free_order[TRACK_MAX];
static int g_free_order_len = 0;
static int g_bad_free = 0;

static void tracker_reset(void) {
    g_alloc_count = 0;
    g_live_count = 0;
    g_free_count = 0;
    g_free_order_len = 0;
    g_bad_free = 0;
    for (int i = 0; i < TRACK_MAX; ++i) g_live[i] = 0;
}

/* A counting allocator: every borrowed node is a slot in g_nodes. */
Node *node_alloc(void) {
    if (g_alloc_count >= TRACK_MAX) return NULL;
    int i = g_alloc_count++;
    g_live[i] = 1;
    ++g_live_count;
    g_nodes[i].value = 0;
    g_nodes[i].next = NULL;
    return &g_nodes[i];
}

void node_free(Node *p) {
    if (p == NULL) return;
    for (int i = 0; i < g_alloc_count; ++i) {
        if (&g_nodes[i] == p) {
            if (!g_live[i]) {
                g_bad_free = 1;
                return;
            }
            g_live[i] = 0;
            --g_live_count;
            ++g_free_count;
            if (g_free_order_len < TRACK_MAX) g_free_order[g_free_order_len++] = i;
            g_nodes[i].next = NULL; /* a returned node no longer holds a link */
            return;
        }
    }
    g_bad_free = 1;
}

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

int main(void) {
    {
        tracker_reset();
        Node *head = NULL;
        for (int i = 1; i <= 5; ++i) head = list_push_front(head, i);
        int out[5] = {0, 0, 0, 0, 0};
        int ok = head != NULL && list_length(head) == 5;
        ok = ok && list_to_array(head, out, 5) == 5;
        for (int i = 0; ok && i < 5; ++i) {
            if (out[i] != 5 - i) ok = 0;
        }
        ok = ok && g_alloc_count == 5 && g_live_count == 5 && !g_bad_free;
        list_free_all(head);
        report("build-order", ok);
    }
    {
        tracker_reset();
        int out[3] = {-1, -1, -1};
        Node *head = NULL;
        for (int i = 1; i <= 5; ++i) head = list_push_front(head, i);
        int ok = list_length(NULL) == 0;
        ok = ok && list_to_array(NULL, out, 3) == 0;
        ok = ok && out[0] == -1 && out[1] == -1 && out[2] == -1;
        ok = ok && list_to_array(head, out, 2) == 2;
        ok = ok && out[0] == 5 && out[1] == 4;
        list_free_all(head);
        report("null-and-cap", ok);
    }
    {
        tracker_reset();
        Node *head = NULL;
        for (int i = 1; i <= 5; ++i) head = list_push_front(head, i);
        int before = g_free_count;
        list_free_all(NULL);
        int ok = g_free_count == before && g_live_count == 5;
        list_free_all(head);
        ok = ok && g_live_count == 0 && g_free_count == 5 && !g_bad_free;
        report("free-all", ok);
    }
    {
        tracker_reset();
        Node *head = NULL;
        for (int i = 1; i <= 5; ++i) head = list_push_front(head, i);
        list_free_all(head);
        int ok = g_free_order_len == 5 && !g_bad_free;
        for (int i = 0; ok && i < 5; ++i) {
            if (g_free_order[i] != 4 - i) ok = 0;
        }
        report("free-order", ok);
    }
    {
        tracker_reset();
        Node *head = list_push_front(NULL, 7);
        int ok = head != NULL && list_length(head) == 1 && head->value == 7;
        while (node_alloc() != NULL) {
            /* take every remaining slot */
        }
        Node *after = list_push_front(head, 8);
        ok = ok && after == head && head->value == 7 && list_length(head) == 1;
        ok = ok && g_bad_free == 0;
        list_free_all(head);
        report("alloc-failure", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
