#include <climits>
#include <vector>

struct SumPolicy {
    static int combine(int left, int right) { return left + right; }
    static int identity() { return 0; }
};

struct MaxPolicy {
    static int combine(int left, int right) {
        return left > right ? left : right;
    }
    static int identity() { return INT_MIN; }
};

template <class Policy>
int reduce(const std::vector<int>& values) {
    int accumulator = Policy::identity();
    for (int value : values) {
        accumulator = Policy::combine(accumulator, value);
    }
    return accumulator;
}
