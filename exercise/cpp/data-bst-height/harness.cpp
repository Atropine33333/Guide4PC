// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool same(const std::vector<int>& actual, const std::vector<int>& expected) {
    return actual == expected;
}

}  // namespace

int main() {
    {
        IntBST tree;
        std::vector<int> values;
        tree.inorder(values);
        report("empty", tree.size() == 0 && tree.height() == 0 &&
                            !tree.contains(1) && values.empty());
    }

    {
        IntBST tree;
        const bool first = tree.insert(5);
        const bool duplicate = tree.insert(5);
        report("single-duplicate", first && !duplicate && tree.size() == 1 &&
                                       tree.height() == 1 && tree.contains(5));
    }

    {
        IntBST tree;
        for (int value : {10, 20, 30, 40}) tree.insert(value);
        report("skewed", tree.size() == 4 && tree.height() == 4);
    }

    {
        IntBST tree;
        for (int value : {50, 30, 70, 20, 40, 60, 80}) tree.insert(value);
        report("balanced", tree.size() == 7 && tree.height() == 3);
    }

    {
        IntBST tree;
        for (int value : {50, 30, 70, 20, 40, 60, 80}) tree.insert(value);
        std::vector<int> values;
        values.push_back(-1);
        tree.inorder(values);
        report("inorder", same(values, {-1, 20, 30, 40, 50, 60, 70, 80}));
    }

    {
        IntBST tree;
        for (int value : {5, 5, 3, 3, 8}) tree.insert(value);
        std::vector<int> values;
        tree.inorder(values);
        report("height-inorder", tree.size() == 3 && tree.height() == 2 &&
                                     same(values, {3, 5, 8}));
    }

    {
        IntBST tree;
        for (int value : {5, 3, 8, 1, 4}) tree.insert(value);
        const bool ok = tree.contains(1) && tree.contains(4) && tree.contains(8) &&
                        !tree.contains(0) && !tree.contains(9);
        report("contains", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
