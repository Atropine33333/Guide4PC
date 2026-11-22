#include <cstddef>

class IntView {
public:
    IntView();
    IntView(int* data, int count);

    int* data() const;
    int size() const;
    bool empty() const;

    int& operator[](int index);
    const int& operator[](int index) const;

    IntView slice(int begin, int count) const;

private:
    int* data_;
    int size_;
};

IntView::IntView() : data_(nullptr), size_(0) {}

IntView::IntView(int* data, int count) : data_(data), size_(count) {
    // TODO: data == nullptr 或 count <= 0 时变成空视图。
}

int* IntView::data() const {
    return data_;
}

int IntView::size() const {
    return size_;
}

bool IntView::empty() const {
    return size_ == 0;
}

int& IntView::operator[](int index) {
    return data_[index];
}

const int& IntView::operator[](int index) const {
    return data_[index];
}

IntView IntView::slice(int begin, int count) const {
    (void)begin;
    (void)count;
    // TODO: 校验范围，返回共享内存的子视图。
    // 暂时的占位实现返回整个视图，保证调用方不会解引用空指针。
    return *this;
}
