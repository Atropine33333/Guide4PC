#include <vector>

struct SumPolicy {
    static int combine(int left, int right) {
        // TODO
        (void)left;
        (void)right;
        return 0;
    }
    static int identity() {
        return 0;  // TODO
    }
};

struct MaxPolicy {
    static int combine(int left, int right) {
        // TODO
        (void)left;
        (void)right;
        return 0;
    }
    static int identity() {
        return 0;  // TODO: 应为 INT_MIN
    }
};

template <class Policy>
int reduce(const std::vector<int>& values) {
    // TODO: 以 Policy::identity() 为初值从左到右归约。
    (void)values;
    return 0;
}
