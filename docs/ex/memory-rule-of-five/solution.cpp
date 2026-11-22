// 参考实现：memory-rule-of-five
#include <cstddef>
#include <utility>

class Resource {
public:
    explicit Resource(int value) : value_(new int(value)) { ++live_; }

    Resource(const Resource& other)
        : value_(other.value_ == nullptr ? nullptr : new int(*other.value_)) {
        if (value_ != nullptr) ++live_;
        ++copies_;
    }

    Resource(Resource&& other) noexcept : value_(other.value_) {
        other.value_ = nullptr;
        ++moves_;
    }

    Resource& operator=(const Resource& other) {
        if (this == &other) return *this;
        int* fresh = other.value_ == nullptr ? nullptr : new int(*other.value_);
        if (value_ != nullptr) {
            delete value_;
            --live_;
        }
        value_ = fresh;
        if (value_ != nullptr) ++live_;
        ++copies_;
        return *this;
    }

    Resource& operator=(Resource&& other) noexcept {
        if (this == &other) return *this;
        if (value_ != nullptr) {
            delete value_;
            --live_;
        }
        value_ = other.value_;
        other.value_ = nullptr;
        ++moves_;
        return *this;
    }

    ~Resource() {
        if (value_ != nullptr) {
            delete value_;
            --live_;
        }
    }

    int value() const { return value_ == nullptr ? -1 : *value_; }

    void set(int value) {
        if (value_ != nullptr) *value_ = value;
    }

    static int live() { return live_; }
    static int copies() { return copies_; }
    static int moves() { return moves_; }

private:
    int* value_ = nullptr;
    inline static int live_ = 0;
    inline static int copies_ = 0;
    inline static int moves_ = 0;
};
