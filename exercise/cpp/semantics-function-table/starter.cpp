#include <string>
#include <vector>

using BinOp = int (*)(int, int);

namespace {
int placeholder(int, int) { return 0; }
}  // namespace

int op_add(int, int) { return 0; }   // TODO
int op_sub(int, int) { return 0; }   // TODO
int op_mul(int, int) { return 0; }   // TODO
int op_min(int, int) { return 0; }   // TODO

BinOp lookup_op(const std::string& name) {
    // TODO: 已知名字返回对应函数，未知名字返回 nullptr。
    (void)name;
    return placeholder;
}

int apply_op(const std::string& name, int a, int b, int fallback) {
    // TODO: 查表；查不到时返回 fallback。
    (void)name;
    (void)a;
    (void)b;
    return fallback;
}

std::vector<int> apply_all(const std::vector<BinOp>& ops, int a, int b) {
    // TODO: 跳过空指针并按顺序收集结果。
    (void)ops;
    (void)a;
    (void)b;
    return {};
}
