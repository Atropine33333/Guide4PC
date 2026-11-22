// 参考实现：memory-growing-array
#include <cstddef>
#include <utility>

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
    : data_(nullptr), size_(0), capacity_(0) {
    if (other.size_ == 0) return;
    data_ = new int[static_cast<std::size_t>(other.size_)];
    for (int i = 0; i < other.size_; ++i) data_[i] = other.data_[i];
    size_ = other.size_;
    capacity_ = other.size_;
}

IntVector& IntVector::operator=(const IntVector& other) {
    if (this == &other) return *this;
    int* fresh = nullptr;
    if (other.size_ > 0) {
        fresh = new int[static_cast<std::size_t>(other.size_)];
        for (int i = 0; i < other.size_; ++i) fresh[i] = other.data_[i];
    }
    delete[] data_;
    data_ = fresh;
    size_ = other.size_;
    capacity_ = other.size_;
    return *this;
}

IntVector::IntVector(IntVector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

IntVector& IntVector::operator=(IntVector&& other) noexcept {
    if (this == &other) return *this;
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
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
