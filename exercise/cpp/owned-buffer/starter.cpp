#include <cstddef>

class IntBuffer {
public:
    explicit IntBuffer(int size) : size_(size), data_(new int[static_cast<std::size_t>(size > 0 ? size : 0)]()) {}

    IntBuffer(const IntBuffer& other) : size_(0), data_(nullptr) {
        (void)other;
        // TODO: 深拷贝 other 的元素。
    }

    IntBuffer& operator=(const IntBuffer& other) {
        (void)other;
        // TODO: 处理自赋值，释放旧内存，再深拷贝。
        return *this;
    }

    ~IntBuffer() { delete[] data_; }

    int size() const {
        // TODO: 返回长度。
        return 0;
    }

    bool set(int index, int value) {
        (void)index;
        (void)value;
        // TODO: 越界返回 false。
        return false;
    }

    bool get(int index, int& out) const {
        (void)index;
        (void)out;
        // TODO: 越界返回 false。
        return false;
    }

    void fill(int value) {
        (void)value;
        // TODO: 填充所有元素。
    }

private:
    int size_ = 0;
    int *data_ = nullptr;
};
