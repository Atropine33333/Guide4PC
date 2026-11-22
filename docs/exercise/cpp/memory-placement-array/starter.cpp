#include <cstddef>
#include <vector>

struct Tracker {
    explicit Tracker(int value) : value_(value) {
        ++alive_;
        ++constructions_;
    }
    ~Tracker() {
        --alive_;
        ++destructions_;
    }

    int value() const { return value_; }

    static int alive() { return alive_; }
    static int constructions() { return constructions_; }
    static int destructions() { return destructions_; }

    Tracker(const Tracker&) = delete;
    Tracker& operator=(const Tracker&) = delete;

private:
    int value_;
    static int alive_;
    static int constructions_;
    static int destructions_;
};

int Tracker::alive_ = 0;
int Tracker::constructions_ = 0;
int Tracker::destructions_ = 0;

class TrackerBuffer {
public:
    explicit TrackerBuffer(std::size_t capacity) : capacity_(capacity) {}

    bool push(int value) {
        // TODO: placement-new an element into raw storage.
        if (values_.size() >= capacity_) return false;
        values_.push_back(value);
        return true;
    }

    std::size_t size() const { return values_.size(); }

    Tracker& operator[](std::size_t index) {
        // TODO: return the constructed object, not a placeholder.
        (void)index;
        return placeholder();
    }

    const Tracker& operator[](std::size_t index) const {
        (void)index;
        return placeholder();
    }

private:
    static Tracker& placeholder() {
        static Tracker value(0);
        return value;
    }

    std::size_t capacity_;
    std::vector<int> values_;
};
