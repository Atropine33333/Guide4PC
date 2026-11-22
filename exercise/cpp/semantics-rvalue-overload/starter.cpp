#include <string>

std::string describe(int value) {
    (void)value;
    // TODO: 只写了一个按值接收的版本，无法区分左值和右值。
    return "lvalue";
}
