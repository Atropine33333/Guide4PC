// 参考实现：semantics-rvalue-overload
#include <string>

std::string describe(int& value) {
    (void)value;
    return "lvalue";
}

std::string describe(const int& value) {
    (void)value;
    return "const-lvalue";
}

std::string describe(int&& value) {
    (void)value;
    return "rvalue";
}
