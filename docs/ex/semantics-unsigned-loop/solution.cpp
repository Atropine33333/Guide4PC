// 参考实现：semantics-unsigned-loop
bool unsigned_less(int a, unsigned b) {
    if (a < 0) return true;
    return static_cast<unsigned>(a) < b;
}

unsigned wrap_decrement(unsigned value, unsigned steps) {
    return value - steps;
}

int countdown_loops(unsigned start) {
    int count = 0;
    unsigned i = start;
    while (i > 0) {
        ++count;
        --i;
    }
    return count;
}
