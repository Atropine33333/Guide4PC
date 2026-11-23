// 参考实现：c-endian-pack
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(void) {
    uint32_t value = 0;
    if (scanf("%" SCNu32, &value) != 1) return 0;

    printf("%02X %02X %02X %02X\n",
           (unsigned)(value & 0xFFu),
           (unsigned)((value >> 8) & 0xFFu),
           (unsigned)((value >> 16) & 0xFFu),
           (unsigned)((value >> 24) & 0xFFu));
    return 0;
}
