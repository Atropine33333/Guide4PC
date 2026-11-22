#include <cstddef>

class IntVector {
public:
    IntVector();
    ~IntVector();
    IntVector(const IntVector& other);
    IntVector& operator=(const IntVector& other);
    IntVector(IntVector&& other) noexcept;
    IntVector& operator=(IntVector&& other) noexcept;

    void push_back(int value);
    int size() const;
    int capacity() const;
    void clear();
    int& operator[](int index);
    const int& operator[](int index) const;
    int* data();
    const int* data() const;

private:
    int* data_;
    int size_;
    int capacity_;
};

IntVector::IntVector() : data_(nullptr), size_(0), capacity_(0) {}

IntVector::~IntVector() {
    delete[] data_;
}

IntVector::IntVector(const IntVector& other)
    : data_(nullptr), size_(other.size_), capacity_(other.size_) {
    // TODO: 深拷贝 other 的所有元素（当前只分配了足够空间）。
    if (capacity_ > 0) {
        data_ = new int[static_cast<std::size_t>(capacity_)]();
    }
}

IntVector& IntVector::operator=(const IntVector& other) {
    (void)other;
    // TODO: 处理自赋值，深拷贝，释放旧内存。
    return *this;
}

IntVector::IntVector(IntVector&& other) noexcept
    : data_(nullptr), size_(0), capacity_(0) {
    (void)other;
    // TODO: 偷走 other 的指针并清空 other。
}

IntVector& IntVector::operator=(IntVector&& other) noexcept {
    (void)other;
    // TODO: 释放旧内存，偷指针，清空 other，注意自移动。
    return *this;
}

void IntVector::push_back(int value) {
    if (size_ == capacity_) {
        const int new_capacity = capacity_ == 0 ? 4 : capacity_ * 2;
        int* fresh = new int[static_cast<std::size_t>(new_capacity)];
        for (int i = 0; i < size_; ++i) fresh[i] = data_[i];
        delete[] data_;
        data_ = fresh;
        capacity_ = new_capacity;
    }
    data_[size_] = value;
    ++size_;
}

int IntVector::size() const {
    return size_;
}

int IntVector::capacity() const {
    return capacity_;
}

void IntVector::clear() {
    size_ = 0;
}

int& IntVector::operator[](int index) {
    return data_[index];
}

const int& IntVector::operator[](int index) const {
    return data_[index];
}

int* IntVector::data() {
    return data_;
}

const int* IntVector::data() const {
    return data_;
}
