// Reference solution: memory-placement-array
#include <cstddef>
#include <new>

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
    explicit TrackerBuffer(std::size_t capacity)
        : capacity_(capacity),
          size_(0),
          storage_(static_cast<std::byte*>(
              ::operator new(capacity * sizeof(Tracker),
                             std::align_val_t{alignof(Tracker)}))) {}

    ~TrackerBuffer() {
        for (std::size_t i = size_; i > 0; --i) {
            slot(i - 1)->~Tracker();
        }
        ::operator delete(storage_, std::align_val_t{alignof(Tracker)});
    }

    TrackerBuffer(const TrackerBuffer&) = delete;
    TrackerBuffer& operator=(const TrackerBuffer&) = delete;

    bool push(int value) {
        if (size_ >= capacity_) return false;
        ::new (static_cast<void*>(storage_ + size_ * sizeof(Tracker)))
            Tracker(value);
        ++size_;
        return true;
    }

    std::size_t size() const { return size_; }

    Tracker& operator[](std::size_t index) {
        return *slot(index);
    }

    const Tracker& operator[](std::size_t index) const {
        return *slot(index);
    }

private:
    Tracker* slot(std::size_t index) {
        return std::launder(reinterpret_cast<Tracker*>(
            storage_ + index * sizeof(Tracker)));
    }

    const Tracker* slot(std::size_t index) const {
        return std::launder(reinterpret_cast<const Tracker*>(
            storage_ + index * sizeof(Tracker)));
    }

    std::size_t capacity_;
    std::size_t size_;
    std::byte* storage_;
};
