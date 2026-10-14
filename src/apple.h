#ifndef APPLE_H
#define APPLE_H

#include "main.h"
#include "game.h"

struct Apple {
    uint32_t pos;
};


bool apple_init(struct Apple **apple);
void apple_eat(struct Game *g);

// randomly select a valid frame_buffer position from the array of valid positions 
uint32_t apple_spawn(struct Game *g);

// add a position to the list of valid positions
bool apple_add_valid_spawn(struct Apple *a, uint32_t position);

// remove a position from the list of valid positions
bool apple_remove_valid_spawn(struct Apple *a, uint32_t position);

#endif
