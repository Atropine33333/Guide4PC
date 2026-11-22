#include <string>
#include <vector>

using BinOp = int (*)(int, int);

int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_min(int a, int b) { return a < b ? a : b; }

BinOp lookup_op(const std::string& name) {
    if (name == "add") return &op_add;
    if (name == "sub") return &op_sub;
    if (name == "mul") return &op_mul;
    if (name == "min") return &op_min;
    return nullptr;
}

int apply_op(const std::string& name, int a, int b, int fallback) {
    const BinOp op = lookup_op(name);
    return op != nullptr ? op(a, b) : fallback;
}

std::vector<int> apply_all(const std::vector<BinOp>& ops, int a, int b) {
    std::vector<int> out;
    out.reserve(ops.size());
    for (BinOp op : ops) {
        if (op != nullptr) {
            out.push_back(op(a, b));
        }
    }
    return out;
}
