// 参考实现：c-bitfield-config
#include <stddef.h>

#define CFG_MODE_MAX 7u
#define CFG_LEVEL_MAX 15u

typedef struct {
    unsigned int mode : 3;
    unsigned int level : 4;
    unsigned int enabled : 1;
} Cfg;

void cfg_init(Cfg *c) {
    c->mode = 0u;
    c->level = 0u;
    c->enabled = 0u;
}

int cfg_set_mode(Cfg *c, unsigned mode) {
    if (c == 0 || mode > CFG_MODE_MAX) return -1;
    c->mode = mode;
    return 0;
}

int cfg_set_level(Cfg *c, unsigned level) {
    if (c == 0 || level > CFG_LEVEL_MAX) return -1;
    c->level = level;
    return 0;
}

int cfg_set_enabled(Cfg *c, int on) {
    if (c == 0 || (on != 0 && on != 1)) return -1;
    c->enabled = (unsigned)on;
    return 0;
}

unsigned cfg_mode(const Cfg *c) {
    if (c == 0) return 0u;
    return (unsigned)c->mode;
}

unsigned cfg_level(const Cfg *c) {
    if (c == 0) return 0u;
    return (unsigned)c->level;
}

int cfg_enabled(const Cfg *c) {
    if (c == 0) return 0;
    return c->enabled != 0u;
}
