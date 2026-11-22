// 参考实现：c-bit-reverse
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint32_t value = 0;
    if (scanf("%" SCNu32, &value) != 1) return 0;

    uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        result = (result << 1) | (value & 1u);
        value >>= 1;
    }

    printf("%" PRIu32 "\n", result);
    return 0;
}
