#include <stdio.h>

enum State {
    IDLE = 0,
    RUN = 2,
    DONE = 5
};

int main(void) {
    int value = 0;
    if (scanf("%d", &value) != 1) return 0;

    /* TODO: 按号码查状态名：0 -> IDLE，2 -> RUN，5 -> DONE，
       其余号码一律 UNKNOWN。 */
    printf("UNKNOWN\n");
    return 0;
}
