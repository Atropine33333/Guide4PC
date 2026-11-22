bool unsigned_less(int a, unsigned b) {
    // TODO: 负数会在这里被转成很大的无符号数，先单独处理 a < 0。
    return a < static_cast<int>(b);
}

unsigned wrap_decrement(unsigned value, unsigned steps) {
    (void)steps;
    // TODO: 无符号减法本身就会按 2^32 回绕。
    return value;
}

int countdown_loops(unsigned start) {
    (void)start;
    // TODO: 数一数 i 从 start 减到 1 一共执行了多少次。
    return 0;
}
