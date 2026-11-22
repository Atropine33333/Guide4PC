#include <functional>

std::function<int()> make_counter(int start) {
    (void)start;
    // TODO: 返回一个每次加一并返回新值的闭包。
    return [] { return 0; };
}

std::function<int(int)> make_accumulator(int start) {
    (void)start;
    // TODO: 返回一个把参数累加到内部总和的闭包。
    return [](int amount) {
        (void)amount;
        return 0;
    };
}
