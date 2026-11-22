// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <string>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
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
        const bool ok = lookup_op("add") != nullptr && lookup_op("add")(7, 5) == 12 &&
                        lookup_op("sub") != nullptr && lookup_op("sub")(7, 5) == 2 &&
                        lookup_op("mul") != nullptr && lookup_op("mul")(-6, 5) == -30 &&
                        lookup_op("min") != nullptr && lookup_op("min")(7, 5) == 5;
        report("lookup-known", ok);
    }

    {
        const bool ok = lookup_op("div") == nullptr && lookup_op("") == nullptr &&
                        lookup_op("ADD") == nullptr;
        report("lookup-unknown", ok);
    }

    {
        const bool ok = apply_op("mul", 9, 8, 111) == 72 &&
                        apply_op("div", 9, 8, 111) == 111 &&
                        apply_op("", 1, 2, -4) == -4;
        report("apply-with-fallback", ok);
    }

    {
        const std::vector<BinOp> ops = {&op_sub, &op_add, nullptr, &op_mul, &op_min};
        report("apply-all-order", same(apply_all(ops, 3, 10), {-7, 13, 30, 3}));
    }

    {
        const std::vector<BinOp> nulls = {nullptr, nullptr};
        const bool ok = apply_all({}, 2, 3).empty() && apply_all(nulls, 2, 3).empty();
        report("apply-all-empty", ok);
    }

    {
        const bool ok = op_mul(1000, 1000) == 1000000 &&
                        op_min(-1000, -999) == -1000 &&
                        op_add(-1000, 1000) == 0;
        report("range-edges", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
