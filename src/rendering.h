#ifndef RENDERING_H
#define RENDERING_H

#include "main.h"
#include "game.h"


bool draw_pixel_xy(struct Game *g, uint32_t x, uint32_t y, uint8_t color);
bool draw_pixel(struct Game *g, uint32_t buffer_index, uint8_t color);
bool renderer_update(struct Game *g);

uint32_t array_to_x(uint32_t array_index);
uint32_t array_to_y(uint32_t array_index);
uint32_t coord_to_array(uint32_t x, uint32_t y);


#endif

