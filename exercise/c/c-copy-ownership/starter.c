#include <stddef.h>

typedef struct {
    char *name;
    int score;
} Player;

Player *player_new(const char *name, int score);
Player *player_clone(const Player *src);
void player_free(Player *p);

Player *player_new(const char *name, int score) {
    /* TODO: 给结构体和名字各申请一块内存，把名字复制进去；失败返回 NULL。 */
    (void)name;
    (void)score;
    return NULL;
}

Player *player_clone(const Player *src) {
    /* TODO: 深拷贝 src，副本的 name 指向另一块内存。 */
    (void)src;
    return NULL;
}

void player_free(Player *p) {
    /* TODO: 先归还名字，再归还结构体；p 为 NULL 时什么也不做。 */
    (void)p;
}
