// 参考实现：templates-type-label
template <typename T>
struct TypeLabel {
    static const char* name() { return "other"; }
    static int rank() { return 0; }
};

template <>
struct TypeLabel<int> {
    static const char* name() { return "int"; }
    static int rank() { return 1; }
};

template <>
struct TypeLabel<double> {
    static const char* name() { return "double"; }
    static int rank() { return 2; }
};

template <>
struct TypeLabel<bool> {
    static const char* name() { return "bool"; }
    static int rank() { return 3; }
};

template <typename T>
struct TypeLabel<T*> {
    static const char* name() { return "pointer"; }
    static int rank() { return 4; }
};
