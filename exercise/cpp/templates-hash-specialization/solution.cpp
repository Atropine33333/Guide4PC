#include <cstddef>
#include <functional>

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
template <>
struct hash<Point> {
    std::size_t operator()(const Point& point) const {
        const std::size_t hx = std::hash<int>{}(point.x);
        const std::size_t hy = std::hash<int>{}(point.y);
        return hx * static_cast<std::size_t>(1000003u) ^ hy;
    }
};
}  // namespace std
