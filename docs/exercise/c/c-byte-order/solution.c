// 参考实现：c-byte-order
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(void) {
    uint32_t value = 0;
    if (scanf("%" SCNx32, &value) != 1) return 0;

    int probe = 1;
    int little = (*(unsigned char *)&probe == 1);
    const unsigned char *p = (const unsigned char *)&value;

    printf("little=%d\n", little);
    printf("bytes=%u %u %u %u\n",
           (unsigned)p[0], (unsigned)p[1], (unsigned)p[2], (unsigned)p[3]);
    return 0;
}
