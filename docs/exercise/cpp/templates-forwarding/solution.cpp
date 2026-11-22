#include <utility>

struct Tracked {
    static int copies;
    static int moves;
    int value;

    explicit Tracked(int value) : value(value) {}
    Tracked(const Tracked& other) : value(other.value) { ++copies; }
    Tracked(Tracked&& other) noexcept : value(other.value) {
        other.value = 0;
        ++moves;
    }
    Tracked& operator=(const Tracked& other) {
        value = other.value;
        return *this;
    }
    Tracked& operator=(Tracked&& other) noexcept {
        value = other.value;
        other.value = 0;
        return *this;
    }
    ~Tracked() = default;
};

int Tracked::copies = 0;
int Tracked::moves = 0;

template <class T>
Tracked forward_to_tracked(T&& value) {
    return Tracked(std::forward<T>(value));
}
