#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(void) {
    uint32_t value = 0;
    if (scanf("%" SCNu32, &value) != 1) return 0;

    (void)value;
    /* TODO: 用移位与掩码取出四个字节，按小端次序输出。 */
    printf("00 00 00 00\n");
    return 0;
}
