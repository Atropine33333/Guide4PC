#include <stdio.h>

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    int sum = 0;
    switch (n) {
    case 1:
        /* TODO: 从 case n 一路写到 case 1，每级只累加本级级数，
           级与级之间不停，落到 case 1 累加完再 break。 */
        break;
    default:
        break;
    }

    printf("%d\n", sum);
    return 0;
}
