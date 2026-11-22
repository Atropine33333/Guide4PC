#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(void) {
    uint32_t value = 0;
    if (scanf("%" SCNx32, &value) != 1) return 0;

    /* TODO: 判断本机是不是小端。 */
    int little = 0;
    (void)value;

    printf("little=%d\n", little);
    /* TODO: 按内存中的字节次序输出四个字节值。 */
    printf("bytes=0 0 0 0\n");
    return 0;
}
