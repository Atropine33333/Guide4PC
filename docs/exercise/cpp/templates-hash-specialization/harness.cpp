// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <unordered_set>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    {
        const Point a{1, 2};
        const Point b{1, 2};
        const Point c{2, 1};
        report("equality", a == b && !(a == c) && !(c == a));
    }

    {
        const Point a{3, 4};
        const Point b{3, 4};
        report("equal-hash", std::hash<Point>{}(a) == std::hash<Point>{}(b));
    }

    {
        const std::size_t h12 = std::hash<Point>{}(Point{1, 2});
        const std::size_t h21 = std::hash<Point>{}(Point{2, 1});
        const std::size_t h34 = std::hash<Point>{}(Point{3, 4});
        const std::size_t h35 = std::hash<Point>{}(Point{3, 5});
        report("hash-both-coordinates", h12 != h21 && h34 != h35 && h34 != h12);
    }

    {
        std::unordered_set<Point> points;
        points.insert(Point{1, 2});
        points.insert(Point{1, 2});
        points.insert(Point{2, 1});
        points.insert(Point{1, 2});
        report("set-dedupe", points.size() == 2 && points.count(Point{1, 2}) == 1 &&
                                 points.count(Point{2, 1}) == 1);
    }

    {
        std::unordered_set<Point> points = {{0, 0}, {5, 6}, {-1, 3}};
        report("set-find", points.count(Point{5, 6}) == 1 &&
                               points.count(Point{-1, 3}) == 1 &&
                               points.count(Point{6, 5}) == 0);
    }

    {
        std::unordered_set<Point> points;
        for (int x = -3; x <= 3; ++x) {
            for (int y = -3; y <= 3; ++y) {
                points.insert(Point{x, y});
            }
        }
        report("negative-coordinates", points.size() == 49);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
