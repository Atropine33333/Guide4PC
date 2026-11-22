// 参考实现：memory-array-view
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
    if (data_ == nullptr || size_ <= 0) {
        data_ = nullptr;
        size_ = 0;
    }
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
    if (begin < 0 || count <= 0) return IntView();
    if (begin >= size_ || count > size_ - begin) return IntView();
    return IntView(data_ + begin, count);
}
