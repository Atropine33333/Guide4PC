#include <vector>

class Histogram {
public:
    Histogram(int buckets, int width)
        : buckets_(buckets),
          width_(width),
          counts_(buckets, 0),
          total_(0),
          overflow_(0) {}

    void add(int value) {
        if (value < 0) {
            return;
        }
        const int bucket = value / width_;
        if (bucket >= buckets_) {
            ++overflow_;
        } else {
            ++counts_[bucket];
        }
        ++total_;
    }

    int buckets() const { return buckets_; }
    int width() const { return width_; }
    int total() const { return total_; }
    int overflow() const { return overflow_; }

    int count(int bucket) const {
        if (bucket < 0 || bucket >= buckets_) {
            return 0;
        }
        return counts_[bucket];
    }

private:
    int buckets_;
    int width_;
    std::vector<int> counts_;
    int total_;
    int overflow_;
};
