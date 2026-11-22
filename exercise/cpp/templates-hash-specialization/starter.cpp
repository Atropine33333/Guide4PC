#include <cstddef>
#include <functional>

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        // TODO
        (void)other;
        return false;
    }
};

namespace std {
template <>
struct hash<Point> {
    std::size_t operator()(const Point& point) const {
        // TODO: 同时使用 x 和 y。
        (void)point;
        return 0;
    }
};
}  // namespace std
