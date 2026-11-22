#include <cstddef>
#include <utility>

class Resource {
public:
    explicit Resource(int value) {
        (void)value;
        // TODO: 没有分配内存，也没有维护 live()。
    }

    Resource(const Resource& other) {
        (void)other;
        // TODO: 没有实现深拷贝。
    }

    Resource(Resource&& other) noexcept {
        (void)other;
        // TODO: 没有接管指针，也没有维护 moves()。
    }

    Resource& operator=(const Resource& other) {
        (void)other;
        // TODO: 没有实现拷贝赋值。
        return *this;
    }

    Resource& operator=(Resource&& other) noexcept {
        (void)other;
        // TODO: 没有实现移动赋值。
        return *this;
    }

    ~Resource() {
        // TODO: 没有释放内存。
    }

    int value() const { return value_ ? *value_ : -1; }

    void set(int value) {
        if (value_) *value_ = value;
    }

    static int live() { return 0; }
    static int copies() { return 0; }
    static int moves() { return 0; }

private:
    int* value_ = nullptr;
};
