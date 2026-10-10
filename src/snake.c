#include "game.h"
#include "rendering.h"

bool snake_init(struct Snake **snake) {
    *snake = calloc(1,sizeof(struct Snake));

    if (*snake == NULL) {
        fprintf(stderr, "Error calloc for Snake failed");
        return false;
    }

    struct Snake *s = *snake;

    s->eating = false;
    s->direction = EAST;
    s->length = 1;
    s->head_pos = (START_Y * CANVAS_WIDTH) + START_X;

    struct Tail *t = calloc(CANVAS_WIDTH*CANVAS_HEIGHT, sizeof(struct Tail));
    s->tails = t;

    t[0].pos = s->head_pos - 1;
    s->bottom_pos = s->tails[s->length-1].pos;

    return true;
}

bool snake_free(struct Snake **snake) {
    if (*snake) {
        free(*snake);
        *snake = NULL;
    }

    return true;
}

void snake_update_position(struct Snake *s, int32_t by_x, int32_t by_y) {
    uint32_t snake_x = array_to_x(s->head_pos);
    uint32_t snake_y = array_to_y(s->head_pos);

    snake_x += by_x;
    snake_y += by_y;

    s->head_pos = coord_to_array(snake_x, snake_y);
}

bool snake_draw(struct Game *g) {
    struct Snake *s = g->snake;

    uint32_t snake_x = array_to_x(s->head_pos);
    uint32_t snake_y = array_to_y(s->head_pos);

    switch (s->direction) {
        case NORTH:
            snake_update_position(s, 0, -1);
            break;
        case EAST:
            snake_update_position(s, 1, 0);
            break;
        case SOUTH:
            snake_update_position(s, 0, 1);
            break;
        case WEST:
            snake_update_position(s, -1, 0);
            break;
    }


    if(!s->eating) { 
        //end of the tail becomes the last tail segment (according to lengths) position before updating
        s->bottom_pos = s->tails[(s->length)-1].pos;
    } else {
        g->snake->eating = false;
    }
    
    //shift all the tails up
    for (int i = s->length - 1; i>0; i--) {
        s->tails[i].pos = s->tails[i-1].pos;
    } 

    s->tails[0].pos = coord_to_array(snake_x, snake_y);

    //position after moving the head
    snake_x = array_to_x(s->head_pos);
    snake_y = array_to_y(s->head_pos);

    if(snake_x == CANVAS_WIDTH-1 || snake_y == CANVAS_HEIGHT-1 || snake_x==0 || snake_y==0) {
        g->is_running=false;
        printf("Game Over! hit wall\n");
    }
    
    for (int i = 0; i<s->length; i++) {
        if(s->tails[i].pos == coord_to_array(snake_x, snake_y)) {
            g->is_running = false;
            printf("Game Over! hit tail\n");
        }
    }

    draw_pixel(g, array_to_x(s->bottom_pos), array_to_y(s->bottom_pos), 0);
    draw_pixel(g, snake_x, snake_y, 255);
    
    return true;
}
