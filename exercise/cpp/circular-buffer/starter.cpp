#include <cstddef>

// 实现一个固定容量的环形缓冲区。
//
//   CircularBuffer buffer(3);
//   buffer.push(1);              // true
//   buffer.push(2); buffer.push(3);
//   buffer.push(4);              // false：已经满了
//   int value;
//   buffer.pop(value);           // true，value == 1
//
// 你需要自己决定用什么成员保存数据、队首下标和元素个数。
class CircularBuffer {
public:
    explicit CircularBuffer(std::size_t capacity) : capacity_(capacity) {}

    bool push(int /*value*/) { return false; }

    bool pop(int & /*out*/) { return false; }

    std::size_t size() const { return 0; }

    std::size_t capacity() const { return capacity_; }

private:
    std::size_t capacity_;
};
