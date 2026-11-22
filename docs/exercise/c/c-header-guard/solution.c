// 参考实现：c-header-guard
#ifndef G4_CHANNEL_H
#define G4_CHANNEL_H

#define CHANNEL_MAX 4
#define CHANNEL_MAGIC 0x5Au

typedef struct {
    int id;
    int gain;
    int open;
} Channel;

int channel_open(Channel *ch, int id, int gain);
int channel_set_gain(Channel *ch, int gain);
int channel_close(Channel *ch);
int channel_is_open(const Channel *ch);
unsigned channel_magic(void);

#endif /* G4_CHANNEL_H */

#ifndef G4_CHANNEL_IMPL_H
#define G4_CHANNEL_IMPL_H

int channel_open(Channel *ch, int id, int gain) {
    if (ch == 0) return -1;
    if (id < 0 || id >= CHANNEL_MAX) return -1;
    if (gain < 0 || gain > 100) return -1;
    ch->id = id;
    ch->gain = gain;
    ch->open = 1;
    return 0;
}

int channel_set_gain(Channel *ch, int gain) {
    if (ch == 0 || ch->open == 0) return -1;
    if (gain < 0 || gain > 100) return -1;
    ch->gain = gain;
    return 0;
}

int channel_close(Channel *ch) {
    if (ch == 0 || ch->open == 0) return -1;
    ch->open = 0;
    return 0;
}

int channel_is_open(const Channel *ch) {
    if (ch == 0) return 0;
    return ch->open != 0;
}

unsigned channel_magic(void) {
    return (unsigned)CHANNEL_MAGIC;
}

#endif /* G4_CHANNEL_IMPL_H */
