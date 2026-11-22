#include <cstddef>

class MinStack {
public:
    void push(int value) {
        (void)value;
        // TODO: 压入元素，同时维护最小值。
    }

    bool pop() {
        // TODO: 栈空返回 false，否则弹出栈顶并返回 true。
        return false;
    }

    bool top(int& out) const {
        (void)out;
        // TODO: 把栈顶写入 out。
        return false;
    }

    bool min(int& out) const {
        (void)out;
        // TODO: 把当前最小值写入 out。
        return false;
    }

    int size() const {
        // TODO: 返回元素个数。
        return 0;
    }
};
