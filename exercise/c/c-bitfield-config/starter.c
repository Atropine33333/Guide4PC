#include <stddef.h>

#define CFG_MODE_MAX 7u
#define CFG_LEVEL_MAX 15u

typedef struct {
    unsigned int mode : 3;
    unsigned int level : 4;
    unsigned int enabled : 1;
} Cfg;

void cfg_init(Cfg *c);
int cfg_set_mode(Cfg *c, unsigned mode);
int cfg_set_level(Cfg *c, unsigned level);
int cfg_set_enabled(Cfg *c, int on);
unsigned cfg_mode(const Cfg *c);
unsigned cfg_level(const Cfg *c);
int cfg_enabled(const Cfg *c);

void cfg_init(Cfg *c) {
    /* TODO: 三个字段全部清 0。 */
    (void)c;
}

int cfg_set_mode(Cfg *c, unsigned mode) {
    /* TODO: 0 到 CFG_MODE_MAX 之间写入并返回 0，否则返回 -1。 */
    (void)c;
    (void)mode;
    return -1;
}

int cfg_set_level(Cfg *c, unsigned level) {
    /* TODO: 0 到 CFG_LEVEL_MAX 之间写入并返回 0，否则返回 -1。 */
    (void)c;
    (void)level;
    return -1;
}

int cfg_set_enabled(Cfg *c, int on) {
    /* TODO: 只接受 0 与 1。 */
    (void)c;
    (void)on;
    return -1;
}

unsigned cfg_mode(const Cfg *c) {
    /* TODO */
    (void)c;
    return 0u;
}

unsigned cfg_level(const Cfg *c) {
    /* TODO */
    (void)c;
    return 0u;
}

int cfg_enabled(const Cfg *c) {
    /* TODO */
    (void)c;
    return 0;
}
