// 参考实现：c-copy-ownership
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int score;
} Player;

Player *player_new(const char *name, int score);
Player *player_clone(const Player *src);
void player_free(Player *p);

Player *player_new(const char *name, int score) {
    if (name == NULL) return NULL;
    size_t len = strlen(name);
    char *copy = (char *)malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, name, len + 1);
    Player *p = (Player *)malloc(sizeof *p);
    if (p == NULL) {
        free(copy);
        return NULL;
    }
    p->name = copy;
    p->score = score;
    return p;
}

Player *player_clone(const Player *src) {
    if (src == NULL || src->name == NULL) return NULL;
    return player_new(src->name, src->score);
}

void player_free(Player *p) {
    if (p == NULL) return;
    free(p->name);
    free(p);
}
