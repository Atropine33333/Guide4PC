// 参考实现：c-enum-state
#include <stdio.h>

enum State {
    IDLE = 0,
    RUN = 2,
    DONE = 5
};

static const char *state_name(int value) {
    switch (value) {
    case IDLE:
        return "IDLE";
    case RUN:
        return "RUN";
    case DONE:
        return "DONE";
    default:
        return "UNKNOWN";
    }
}

int main(void) {
    int value = 0;
    if (scanf("%d", &value) != 1) return 0;

    printf("%s\n", state_name(value));
    return 0;
}
