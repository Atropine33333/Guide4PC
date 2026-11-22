#include <stddef.h>

#define FLAGS_WORD_MAX 0xFFu

typedef struct {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 5;
} Flags;

void flags_init(Flags *f);
int flags_set(Flags *f, char which, unsigned value);
unsigned flags_get(const Flags *f, char which);
unsigned flags_pack(const Flags *f);
int flags_unpack(Flags *f, unsigned word);

void flags_init(Flags *f) {
    /* TODO: 三个字段全部清 0。 */
    (void)f;
}

int flags_set(Flags *f, char which, unsigned value) {
    /* TODO: 字段名 a、b、c 分别接受 0..1、0..3、0..31；越界或不认识返回 -1。 */
    (void)f;
    (void)which;
    (void)value;
    return -1;
}

unsigned flags_get(const Flags *f, char which) {
    /* TODO: 返回对应字段的值；字段名不认识返回 0。 */
    (void)f;
    (void)which;
    return 0u;
}

unsigned flags_pack(const Flags *f) {
    /* TODO: a 放第 0 位，b 放第 1..2 位，c 放第 3..7 位。 */
    (void)f;
    return 0u;
}

int flags_unpack(Flags *f, unsigned word) {
    /* TODO: word 不超过 FLAGS_WORD_MAX 就按同样的位序写进三个字段；否则返回 -1。 */
    (void)f;
    (void)word;
    return -1;
}
