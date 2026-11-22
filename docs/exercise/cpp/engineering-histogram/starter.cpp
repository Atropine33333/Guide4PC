#include <vector>

class Histogram {
public:
    Histogram(int buckets, int width)
        : buckets_(buckets), width_(width), counts_(buckets, 0), total_(0), overflow_(0) {}

    void add(int value) {
        // TODO: 负值忽略；越界计入 overflow；否则对应桶加一。
        (void)value;
    }

    int buckets() const { return buckets_; }
    int width() const { return width_; }
    int total() const {
        (void)total_;
        return 0;  // TODO
    }
    int overflow() const {
        (void)overflow_;
        return 0;  // TODO
    }

    int count(int bucket) const {
        // TODO: 越界返回 0。
        (void)bucket;
        return 0;
    }

private:
    int buckets_;
    int width_;
    std::vector<int> counts_;
    int total_;
    int overflow_;
};
