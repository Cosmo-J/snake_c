#include "apple.h"
#include "rendering.h"

bool apple_init(struct Apple **apple) {
    *apple = calloc(1,sizeof(struct Apple));
    if(*apple == NULL) {
        fprintf(stderr, "Error calloc for framebuffer.\n");
        return false;
    }
    
    struct Apple *a = *apple;

    //TODO fix magic number
    a->pos = (START_Y * CANVAS_WIDTH) + START_X + 4;
    return true;
}


// randomly select a valid frame_buffer position from the array of valid positions 
uint32_t apple_spawn(struct Game *g) {
    uint32_t pos_candidate = 0;
    bool found_spawn = false;

    uint32_t attempts = 0;
    while (!found_spawn) {
        if (attempts >= g->l_framebuffer) {
            //tried all possible conditions
            // backup win condition
            printf("You Win!");
            g->is_running = false;
        }

        pos_candidate = rand() % g->l_framebuffer; 
        found_spawn = (g->framebuffer[pos_candidate] == 0); // when the potential spawn location is a white space
        attempts++;
    } 
    
    return pos_candidate;
}




