#include "game.h"
#include "rendering.h"

bool apple_init(struct Apple **apple) {
    *apple = calloc(1,sizeof(struct Apple));

    if(*apple == NULL) {
        fprintf(stderr, "Error calloc for framebuffer.\n");
        return false;
    }
    
    struct Apple *a = *apple;
    
    int x = 1 + rand() % (CANVAS_WIDTH - 2);
    int y = 1 + rand() % (CANVAS_HEIGHT - 2);

    a->pos = y * CANVAS_WIDTH + x;

    return true;
}
