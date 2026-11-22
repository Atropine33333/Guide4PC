/* TODO: 用 #ifndef G4_CHANNEL_H / #define G4_CHANNEL_H / #endif 把下面的声明块包起来。 */

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

/* TODO: 用 #ifndef G4_CHANNEL_IMPL_H / #define G4_CHANNEL_IMPL_H / #endif 把下面的实现包起来，并补完这些函数。 */

int channel_open(Channel *ch, int id, int gain) {
    /* TODO */
    (void)ch;
    (void)id;
    (void)gain;
    return -1;
}

int channel_set_gain(Channel *ch, int gain) {
    /* TODO */
    (void)ch;
    (void)gain;
    return -1;
}

int channel_close(Channel *ch) {
    /* TODO */
    (void)ch;
    return -1;
}

int channel_is_open(const Channel *ch) {
    /* TODO */
    (void)ch;
    return 0;
}

unsigned channel_magic(void) {
    /* TODO */
    return 0u;
}
