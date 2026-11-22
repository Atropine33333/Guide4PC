// 参考实现：semantics-lambda-state
#include <functional>

std::function<int()> make_counter(int start) {
    return [value = start]() mutable { return ++value; };
}

std::function<int(int)> make_accumulator(int start) {
    return [total = start](int amount) mutable {
        total += amount;
        return total;
    };
}
