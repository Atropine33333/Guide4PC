#include <stdio.h>
#include <stdint.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    uint8_t acc = 0;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        if (scanf("%d", &value) != 1) break;

        /* TODO: 把 value 加到 uint8_t 累加器上（超出 255 时回绕），
           然后输出当前累加值，每行一个。 */
        (void)value;
        (void)acc;
    }

    return 0;
}
