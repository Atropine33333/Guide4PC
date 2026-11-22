#include <vector>

class TwoStackQueue {
public:
    void push(int value) { in_.push_back(value); }

    int front() const {
        if (!out_.empty()) {
            return out_.back();
        }
        if (!in_.empty()) {
            return in_.front();
        }
        return -1;
    }

    void pop() {
        if (out_.empty()) {
            while (!in_.empty()) {
                out_.push_back(in_.back());
                in_.pop_back();
            }
        }
        if (!out_.empty()) {
            out_.pop_back();
        }
    }

    bool empty() const { return in_.empty() && out_.empty(); }

    int size() const {
        return static_cast<int>(in_.size() + out_.size());
    }

private:
    std::vector<int> in_;
    std::vector<int> out_;
};
