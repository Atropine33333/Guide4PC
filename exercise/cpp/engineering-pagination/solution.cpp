// 参考实现：engineering-pagination
#include <algorithm>
#include <cstddef>
#include <vector>

class Pager {
public:
    Pager(int total_items, int page_size, int current_page)
        : total_(total_items < 0 ? 0 : total_items),
          size_(page_size < 1 ? 1 : page_size),
          current_(1) {
        const int pages = page_count();
        if (pages == 0) {
            current_ = 1;
        } else {
            current_ = std::clamp(current_page, 1, pages);
        }
    }

    int page_count() const { return (total_ + size_ - 1) / size_; }

    int current() const { return current_; }

    bool has_prev() const { return current_ > 1; }

    bool has_next() const { return current_ < page_count(); }

    int first_index() const {
        if (total_ == 0) return -1;
        return (current_ - 1) * size_;
    }

    int last_index() const {
        if (total_ == 0) return -1;
        const int last = first_index() + size_ - 1;
        return std::min(last, total_ - 1);
    }

    std::vector<int> window(int radius) const {
        std::vector<int> pages;
        const int pages_total = page_count();
        if (pages_total == 0) return pages;
        const int r = radius < 0 ? 0 : radius;
        const int first = std::max(1, current_ - r);
        const int last = std::min(pages_total, current_ + r);
        for (int page = first; page <= last; ++page) pages.push_back(page);
        return pages;
    }

private:
    int total_ = 0;
    int size_ = 1;
    int current_ = 1;
};
