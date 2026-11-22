#include <cstddef>
#include <vector>

class MinHeap {
public:
    void push(int value) {
        // TODO: 没有上浮，最小值不一定在堆顶。
        data_.push_back(value);
    }

    int pop() {
        // TODO: 直接拿走了第一个元素，没有下沉调整。
        const int value = data_.front();
        data_.erase(data_.begin());
        return value;
    }

    int top() const { return data_.front(); }

    std::size_t size() const { return data_.size(); }

    bool empty() const { return data_.empty(); }

private:
    std::vector<int> data_;
};
