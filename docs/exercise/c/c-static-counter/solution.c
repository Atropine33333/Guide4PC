// 参考实现：c-static-counter
static int counter = 0;

void counter_reset(void) {
    counter = 0;
}

int counter_next(void) {
    return ++counter;
}

int counter_add(int delta) {
    counter += delta;
    return counter;
}

int counter_get(void) {
    return counter;
}
