#include <algorithm>
#include <vector>

class TwoStackQueue {
public:
    void push(int value) {
        in_.push_back(value);
        in_min_.push_back(in_min_.empty() ? value : std::min(value, in_min_.back()));
    }

    int front() const {
        if (!out_.empty()) return out_.back();
        if (!in_.empty()) return in_.front();
        return -1;
    }

    void pop() {
        if (out_.empty()) {
            while (!in_.empty()) {
                const int value = in_.back();
                in_.pop_back();
                in_min_.pop_back();
                out_.push_back(value);
                out_min_.push_back(out_min_.empty() ? value : std::min(value, out_min_.back()));
            }
        }
        if (!out_.empty()) {
            out_.pop_back();
            out_min_.pop_back();
        }
    }

    bool empty() const { return in_.empty() && out_.empty(); }

    int size() const {
        return static_cast<int>(in_.size() + out_.size());
    }

    int min() const {
        if (empty()) return -1;
        if (out_.empty()) return in_min_.back();
        if (in_.empty()) return out_min_.back();
        return std::min(in_min_.back(), out_min_.back());
    }

private:
    std::vector<int> in_;
    std::vector<int> out_;
    std::vector<int> in_min_;
    std::vector<int> out_min_;
};
