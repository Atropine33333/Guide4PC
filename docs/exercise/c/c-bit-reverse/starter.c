#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint32_t value = 0;
    if (scanf("%" SCNu32, &value) != 1) return 0;

    uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        /* TODO: 把 value 当前最低位接到 result 的末尾，再把 value 右移一位。 */
        (void)value;
    }

    printf("%" PRIu32 "\n", result);
    return 0;
}
