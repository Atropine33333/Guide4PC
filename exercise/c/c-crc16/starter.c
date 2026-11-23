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

    (void)len;
    /* TODO: 按题面的初值与多项式，逐字节计算 CRC16。 */
    printf("CRC16=0x0000\n");
    return 0;
}
