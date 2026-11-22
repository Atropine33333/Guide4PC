// 参考实现：min-stack
#include <cstddef>
#include <vector>

class MinStack {
public:
    void push(int value) {
        values_.push_back(value);
        if (minimums_.empty() || value <= minimums_.back()) {
            minimums_.push_back(value);
        }
    }

    bool pop() {
        if (values_.empty()) return false;
        if (values_.back() == minimums_.back()) {
            minimums_.pop_back();
        }
        values_.pop_back();
        return true;
    }

    bool top(int& out) const {
        if (values_.empty()) return false;
        out = values_.back();
        return true;
    }

    bool min(int& out) const {
        if (minimums_.empty()) return false;
        out = minimums_.back();
        return true;
    }

    int size() const { return static_cast<int>(values_.size()); }

private:
    std::vector<int> values_;
    std::vector<int> minimums_;
};
