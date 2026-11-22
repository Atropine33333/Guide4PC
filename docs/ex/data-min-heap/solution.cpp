// 参考实现：data-min-heap
#include <cstddef>
#include <utility>
#include <vector>

class MinHeap {
public:
    void push(int value) {
        data_.push_back(value);
        std::size_t index = data_.size() - 1;
        while (index > 0) {
            const std::size_t parent = (index - 1) / 2;
            if (data_[parent] <= data_[index]) break;
            std::swap(data_[parent], data_[index]);
            index = parent;
        }
    }

    int pop() {
        const int result = data_.front();
        data_.front() = data_.back();
        data_.pop_back();
        std::size_t index = 0;
        while (true) {
            const std::size_t left = index * 2 + 1;
            const std::size_t right = left + 1;
            std::size_t smallest = index;
            if (left < data_.size() && data_[left] < data_[smallest]) smallest = left;
            if (right < data_.size() && data_[right] < data_[smallest]) smallest = right;
            if (smallest == index) break;
            std::swap(data_[index], data_[smallest]);
            index = smallest;
        }
        return result;
    }

    int top() const { return data_.front(); }

    std::size_t size() const { return data_.size(); }

    bool empty() const { return data_.empty(); }

private:
    std::vector<int> data_;
};
