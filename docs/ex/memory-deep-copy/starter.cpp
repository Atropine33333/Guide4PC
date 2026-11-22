#include <cstddef>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t size) : size_(size) {
        // TODO: 没有分配内存，也没有把元素置 0。
    }

    // TODO: 还没有写拷贝构造函数，默认拷贝会让两个对象共用同一块内存。

    // TODO: 还没有写赋值运算符。

    ~IntBuffer() {
        // TODO: 没有释放内存。
    }

    std::size_t size() const { return size_; }

    int& at(std::size_t index) { return data_[index]; }
    const int& at(std::size_t index) const { return data_[index]; }

private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
};
