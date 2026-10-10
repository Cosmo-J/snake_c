#ifndef RENDERING_H
#define RENDERING_H

#include "main.h"
#include "game.h"


bool draw_pixel(struct Game *g, uint32_t x, uint32_t y, uint8_t color);
bool renderer_update(struct Game *g);

#endif

