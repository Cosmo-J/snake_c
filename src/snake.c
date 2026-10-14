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
    s->moving = false;
    s->direction = EAST;
    s->l_move_buffer = 2;
    s->move_buffer[0] = NoDir;
    s->move_buffer[1] = NoDir;
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


//returns the opposite direction of a given cardinal
Cardinal invert_cardinal(Cardinal direction) {
    Cardinal inverse;
    if (direction==NORTH) inverse = SOUTH;
    else if (direction==SOUTH) inverse = NORTH;
    else if (direction==EAST) inverse = WEST;
    else if (direction==WEST) inverse = EAST;
    else inverse = NoDir;

    return inverse;
}


//translates an input to a direction and adds it to the movement buffer
void snake_set_direction(struct Snake *s, Cardinal inputed_dir) {
    // this could be sped up if neccesary by creating a struct which stores the next available buffer index thus going from O(n) to O(1)
    //      Would be overkill though because the buffer wont ever be that big
    for (int i=0; i < (int)s->l_move_buffer; i++) {
        if (s->move_buffer[i] == NoDir) {
            s->move_buffer[i] = inputed_dir;
            break;
        }
    }
}

//gets a direction from the buffer and assigns it to the current direction
//shifts the movement buffer along one
//  id1 goes to id0, id1 become NoDir unless another input has been added
Cardinal snake_get_direction(struct Snake *s) {
    Cardinal got_direction = s->move_buffer[0];
    
    //shift buffer
    int last_index = s->l_move_buffer - 1;
    for (int i = 0; i <= last_index; i++) {
        int next_i = i+1;
        if (next_i > last_index) {
            s->move_buffer[i] = NoDir;
        }
        else {
            s->move_buffer[i] = s->move_buffer[next_i];
        }
    }
    
    //if invalid input, return no direction
    if (invert_cardinal(s->direction) == got_direction) {
        got_direction = NoDir;
    }
    return got_direction;
}



void snake_update_position(struct Snake *s, Cardinal direction) {
    uint32_t snake_x = array_to_x(s->head_pos);
    uint32_t snake_y = array_to_y(s->head_pos);

    int32_t by_x;
    int32_t by_y;

    switch (direction) {
        case NORTH:
            by_x = 0;
            by_y = -1;
            break;
        case EAST:
            by_x = 1;
            by_y = 0;
            break;
        case SOUTH:
            by_x = 0;
            by_y = 1;
            break;
        case WEST:
            by_x = -1;
            by_y = 0;
            break;
        case NoDir:
            by_x = 0;
            by_y = 0;
            break;
    }

    snake_x += by_x;
    snake_y += by_y;

    s->head_pos = coord_to_array(snake_x, snake_y);
}

bool snake_lost(struct Snake *s) {
    if (s->head_pos % CANVAS_WIDTH == 0 || s->head_pos % CANVAS_WIDTH == CANVAS_WIDTH - 1 || s->head_pos / CANVAS_WIDTH == 0 || s->head_pos / CANVAS_WIDTH == CANVAS_HEIGHT - 1) {
        printf("Game Over! hit wall\n");
        return true;
    }
    
    for (int i = 0; i<s->length; i++) {
        if(s->tails[i].pos == s->head_pos) {
            printf("Game Over! hit tail\n");
            return true;
        }
    }
    return false;
}


bool snake_draw(struct Game *g) {
    struct Snake *s = g->snake;

    // snake position prior to being updated
    uint32_t prior_snake_pos = s->head_pos;

    //set_direction is done before in the main loop with the events
    Cardinal new_direction = snake_get_direction(s);

    if (new_direction != NoDir) {
        s->direction = new_direction;
    }

    snake_update_position(s, s->direction);


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

    // assign the head tail to the prior position of the snake head
    s->tails[0].pos = prior_snake_pos;

    if(snake_lost(s)) {
         g->is_running = false;
    } 

    draw_pixel(g, s->bottom_pos, 0);
    draw_pixel(g, s->head_pos, 255);

    return true;
}
