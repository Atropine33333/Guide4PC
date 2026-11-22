template <typename T, int Capacity>
class BoundedStack {
public:
    bool push(const T& value) {
        (void)value;
        // TODO: 未满时保存元素并返回 true。
        return false;
    }

    bool pop() {
        // TODO: 非空时弹出栈顶。
        return false;
    }

    bool top(T& out) const {
        (void)out;
        // TODO: 非空时把栈顶写入 out。
        return false;
    }

    int size() const {
        // TODO: 返回元素个数。
        return 0;
    }

    static constexpr int capacity() { return Capacity; }
};
