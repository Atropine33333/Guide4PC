#include <string>

std::string to_base(long long value, int base) {
    (void)value;
    (void)base;
    // TODO: 逐位取余并倒序拼接，注意 0 和 LLONG_MIN。
    return "";
}

bool parse_base(const std::string& text, int base, long long& out) {
    (void)text;
    (void)base;
    (void)out;
    // TODO: 处理符号、逐位累加，并检查非法字符与溢出。
    return false;
}
