// Guide4PC harness appended to the student's code in the same translation unit.
#include <stdio.h>

static int g_total = 0;
static int g_failed = 0;

static void report(const char *id, int ok) {
    ++g_total;
    if (!ok) ++g_failed;
    printf("@@G4PC {\"t\":\"check\",\"id\":\"%s\",\"ok\":%s}\n", id, ok ? "true" : "false");
}

static int is_heap(const MinHeap *h) {
    for (size_t i = 1; i < h->len; ++i) {
        if (h->data[(i - 1) / 2] > h->data[i]) return 0;
    }
    return 1;
}

static void sort_ints(int *a, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        int v = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > v) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = v;
    }
}

static int same_multiset(const MinHeap *h, const int *expect, size_t n) {
    if (h->len != n) return 0;
    int copy[HEAP_CAP];
    for (size_t i = 0; i < n; ++i) copy[i] = h->data[i];
    sort_ints(copy, n);
    for (size_t i = 0; i < n; ++i) {
        if (copy[i] != expect[i]) return 0;
    }
    return 1;
}

int main(void) {
    {
        static const int values[10] = {9, 4, 7, 1, 8, 2, 6, 3, 5, 0};
        static const int sorted[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        MinHeap h;
        heap_build(&h, values, 10);
        report("build-property", h.len == 10 && is_heap(&h) && same_multiset(&h, sorted, 10) &&
                                    h.data[0] == 0);
    }
    {
        MinHeap h;
        static const int sorted[7] = {1, 2, 3, 4, 5, 6, 100};
        h.len = 7;
        h.data[0] = 100;
        h.data[1] = 1;
        h.data[2] = 2;
        h.data[3] = 3;
        h.data[4] = 4;
        h.data[5] = 5;
        h.data[6] = 6;
        heap_sift_down(&h, 0);
        report("sift-repairs", h.len == 7 && is_heap(&h) && h.data[0] == 1 &&
                                   same_multiset(&h, sorted, 7));
    }
    {
        static const int values[7] = {5, 3, 8, 1, 9, 2, 7};
        static const int order[7] = {1, 2, 3, 5, 7, 8, 9};
        MinHeap h;
        int v = 0;
        heap_build(&h, values, 7);
        int ok = 1;
        for (int i = 0; i < 7; ++i) {
            if (heap_extract_min(&h, &v) != 0 || v != order[i]) ok = 0;
        }
        ok = ok && h.len == 0 && heap_extract_min(&h, &v) == -1 && v == 9;
        report("extract-sorted", ok);
    }
    {
        static const int values[6] = {4, 4, 2, 2, 9, 9};
        static const int order[6] = {2, 2, 4, 4, 9, 9};
        MinHeap h;
        int v = 0;
        heap_build(&h, values, 6);
        int ok = is_heap(&h);
        for (int i = 0; i < 6; ++i) {
            if (heap_extract_min(&h, &v) != 0 || v != order[i]) ok = 0;
        }
        ok = ok && h.len == 0;
        report("duplicates", ok);
    }
    {
        MinHeap h;
        int v = 55;
        static const int none[1] = {0};
        heap_build(&h, none, 0);
        int ok = h.len == 0 && heap_extract_min(&h, &v) == -1 && v == 55;
        static const int one[1] = {42};
        heap_build(&h, one, 1);
        ok = ok && h.len == 1 && h.data[0] == 42 && v == 55;
        ok = ok && heap_extract_min(&h, &v) == 0 && v == 42;
        ok = ok && h.len == 0 && heap_extract_min(&h, &v) == -1 && v == 42;
        report("empty-and-single", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
