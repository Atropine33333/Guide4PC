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
        int out[4] = {0, 0, 0, 0};
        report("empty-tree", bst_search(NULL, 5) == 0 && bst_count(NULL) == 0 &&
                                bst_inorder(NULL, out, 4) == 0);
    }
    {
        static const int keys[7] = {50, 30, 70, 20, 40, 60, 80};
        BstNode *root = NULL;
        for (int i = 0; i < 7; ++i) root = bst_insert(root, keys[i]);
        int ok = root != NULL && bst_count(root) == 7;
        for (int i = 0; i < 7; ++i) {
            if (bst_search(root, keys[i]) != 1) ok = 0;
        }
        ok = ok && bst_search(root, 35) == 0 && bst_search(root, 45) == 0 &&
             bst_search(root, 10) == 0;
        bst_free(root);
        report("insert-search", ok);
    }
    {
        static const int keys[7] = {50, 30, 70, 20, 40, 60, 80};
        static const int want[7] = {20, 30, 40, 50, 60, 70, 80};
        int out[16];
        BstNode *root = NULL;
        for (int i = 0; i < 7; ++i) root = bst_insert(root, keys[i]);
        size_t n = bst_inorder(root, out, 16);
        int ok = n == 7;
        for (int i = 0; i < 7 && ok; ++i) {
            if (out[i] != want[i]) ok = 0;
        }
        int first[4] = {0, 0, 0, 0};
        ok = ok && bst_inorder(root, first, 4) == 4;
        ok = ok && first[0] == 20 && first[1] == 30 && first[2] == 40 && first[3] == 50;
        bst_free(root);
        report("inorder-sorted", ok);
    }
    {
        static const int keys[5] = {5, 5, 3, 5, 7};
        BstNode *root = NULL;
        for (int i = 0; i < 5; ++i) root = bst_insert(root, keys[i]);
        int ok = bst_count(root) == 3 && bst_search(root, 5) == 1 &&
                 bst_search(root, 3) == 1 && bst_search(root, 7) == 1;
        bst_free(root);
        report("duplicates", ok);
    }
    {
        int out[16];
        BstNode *root = NULL;
        bst_free(NULL);
        for (int i = 1; i <= 10; ++i) root = bst_insert(root, i);
        bst_free(root);
        root = NULL;
        root = bst_insert(root, 9);
        root = bst_insert(root, 5);
        root = bst_insert(root, 12);
        size_t n = bst_inorder(root, out, 16);
        int ok = bst_count(root) == 3 && n == 3;
        ok = ok && out[0] == 5 && out[1] == 9 && out[2] == 12;
        bst_free(root);
        report("free-and-rebuild", ok);
    }
    printf("@@G4PC {\"t\":\"done\",\"total\":%d,\"failed\":%d}\n", g_total, g_failed);
    return 0;
}
