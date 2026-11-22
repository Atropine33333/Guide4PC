#include <vector>

class TwoStackQueue {
public:
    void push(int value) {
        // TODO: 放入接收栈。
        (void)value;
    }

    int front() const {
        // TODO: 空队列返回 -1，否则返回队首。
        return -1;
    }

    void pop() {
        // TODO: 删除队首；必要时把接收栈倒入输出栈。
    }

    bool empty() const {
        return true;  // TODO
    }

    int size() const {
        return 0;  // TODO
    }

    int min() const {
        // TODO: 空队列返回 -1，否则返回队内最小值。
        return -1;
    }

private:
    std::vector<int> in_;
    std::vector<int> out_;
};
