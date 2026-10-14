#ifndef GAME_H
#define GAME_H

#include "main.h"

#define TARGET_FPS 1.0/60
#define GAME_DELAY_MS 80

#define CANVAS_WIDTH 32 
#define CANVAS_HEIGHT 32
#define CANVAS_PITCH CANVAS_WIDTH 

#define START_X CANVAS_WIDTH/2
#define START_Y CANVAS_HEIGHT/2


typedef enum CARDINAL_ENUM{
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NoDir
} Cardinal;

struct Tail {
    uint32_t pos;    
};


struct Apple {
    uint32_t pos; 
};


struct Snake {
    int length; 
    uint32_t head_pos;
    uint32_t bottom_pos;
    struct Tail *tails;
    bool eating; // true after the frame an apple is eaten, used to delay the tails by 1, in effect extended the tails
    bool moving;
    Cardinal direction;
    Cardinal move_buffer[2];
    size_t l_move_buffer;
};


struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *canvas;
  uint8_t *framebuffer;
  size_t l_framebuffer;
  SDL_Event event;
  bool is_running;
  struct Snake *snake;
  struct Apple *apple;
};


bool game_init_sdl(struct Game *g);

void game_run(struct Game *g);
void game_free(struct Game **game);
bool game_new(struct Game **game);

uint32_t array_to_x(uint32_t array_index);
uint32_t array_to_y(uint32_t array_index);
uint32_t coord_to_array(uint32_t x, uint32_t y);


bool snake_init(struct Snake **snake);
bool snake_draw(struct Game *g);
void snake_set_direction(struct Snake *s, Cardinal inputed_dir);
Cardinal snake_get_direction(struct Snake *s);

bool apple_init(struct Apple **apple);
void apple_eat(struct Game *g);

#endif
