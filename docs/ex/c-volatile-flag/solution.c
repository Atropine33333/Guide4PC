// 参考实现：c-volatile-flag
volatile int flag = 0;

void set_flag(void) {
    flag = 1;
}

void clear_flag(void) {
    flag = 0;
}

int wait_until_set(int limit) {
    for (int i = 0; i < limit; ++i) {
        if (flag != 0) {
            return 1;
        }
    }
    return 0;
}
