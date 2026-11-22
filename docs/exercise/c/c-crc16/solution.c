// 参考实现：c-crc16
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(void) {
    char line[202];
    if (fgets(line, sizeof line, stdin) == NULL) {
        line[0] = '\0';
    }

    size_t len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        line[--len] = '\0';
    }

    uint16_t crc = 0xFFFFu;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)((unsigned char)line[i] << 8);
        for (int bit = 0; bit < 8; ++bit) {
            if (crc & 0x8000u) {
                crc = (uint16_t)((crc << 1) ^ 0x1021u);
            } else {
                crc = (uint16_t)(crc << 1);
            }
        }
    }

    printf("CRC16=0x%04X\n", (unsigned)crc);
    return 0;
}
