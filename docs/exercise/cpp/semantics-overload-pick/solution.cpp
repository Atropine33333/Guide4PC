// 参考实现：semantics-overload-pick
const char* pick(int value) {
    (void)value;
    return "int";
}

const char* pick(long value) {
    (void)value;
    return "long";
}

const char* pick(double value) {
    (void)value;
    return "double";
}
