template <typename T>
struct TypeLabel {
    static const char* name() {
        return "other";
    }

    static int rank() {
        return 0;
    }
};

// TODO: 为 int、double、bool 和指针类型添加特化。
