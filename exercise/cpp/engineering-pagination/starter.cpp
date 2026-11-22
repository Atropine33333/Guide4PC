#include <algorithm>
#include <cstddef>
#include <vector>

class Pager {
public:
    Pager(int total_items, int page_size, int current_page)
        : total_(total_items), size_(page_size), current_(current_page) {
        // TODO: 没有钳制 current_，也没有处理空数据。
    }

    int page_count() const {
        if (size_ <= 0) return 0;
        return (total_ + size_ - 1) / size_;
    }

    int current() const { return current_; }
    bool has_prev() const { return current_ > 1; }
    bool has_next() const { return current_ < page_count(); }

    int first_index() const {
        // TODO: 空数据时也应返回 -1。
        return (current_ - 1) * size_;
    }

    int last_index() const {
        // TODO: 还没有按 total_ 截断。
        return first_index() + size_ - 1;
    }

    std::vector<int> window(int radius) const {
        // TODO: 返回空窗口。
        (void)radius;
        return {};
    }

private:
    int total_ = 0;
    int size_ = 1;
    int current_ = 1;
};
