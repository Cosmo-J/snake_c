#include "game.h"
#include "rendering.h"

uint32_t array_to_x(uint32_t array_index) {
    return array_index % CANVAS_WIDTH;
}


uint32_t array_to_y(uint32_t array_index) {
    return array_index / CANVAS_WIDTH;
}


uint32_t coord_to_array(uint32_t x, uint32_t y) {
   return (y * CANVAS_WIDTH) + x;  
}


bool game_init_sdl(struct Game *g) {

    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initialising SDL3: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(!g->window) {
        fprintf(stderr, "Error creating SDL_Window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, NULL);
    if(!g->renderer) {
        fprintf(stderr, "Error creating SDL_Renderer: %s\n", SDL_GetError());
        return false;
    }
    g->canvas = SDL_CreateTexture(g->renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, CANVAS_WIDTH,CANVAS_HEIGHT);
    if(!g->canvas) {
        fprintf(stderr, "Error creating SDL_Texture: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetTextureScaleMode(g->canvas, SDL_SCALEMODE_NEAREST);
    SDL_SetRenderLogicalPresentation(g->renderer,CANVAS_WIDTH, CANVAS_HEIGHT,SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return true;
}


bool game_new(struct Game **game) {
    *game = calloc(1,sizeof(struct Game));
    struct Game *g = *game;


    if(g == NULL) {
        fprintf(stderr, "Error in Calloc of a new Game.\n");
        return false;
    }


    if(!game_init_sdl(g)) {
        return false;
    }


    g->framebuffer = calloc(CANVAS_WIDTH * CANVAS_HEIGHT, sizeof(uint8_t));
    if(g->framebuffer == NULL) {
        fprintf(stderr, "Error calloc for framebuffer.\n");
        return false;
    }

    struct Snake *snake = NULL;  
    if (!snake_init(&snake)) {
       return false; 
    }
    g->snake = snake;

    g->is_running = true;
    return true;
}


void game_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;

        if (g->canvas) {
            SDL_DestroyTexture(g->canvas);
            g->canvas = NULL;
        }

        if (g->framebuffer) {
            free(g->framebuffer);
            g->framebuffer=NULL;
        }


        if (g->window) {
            SDL_DestroyWindow(g->window);
            g->window = NULL;
        }

       if (g->renderer) {
            SDL_DestroyRenderer(g->renderer);
            g->renderer = NULL;
       }

       SDL_Quit();

       free(g);
       g=NULL;
       *game = NULL;

       printf("All Clean!\n");
    }    
}


void game_events(struct Game *g) {
    while (SDL_PollEvent(&g->event)) {
        switch (g->event.type) {
        case SDL_EVENT_QUIT:
            g->is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (g->event.key.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    g->is_running = false;
                break;
                case SDL_SCANCODE_W:
                    if(g->snake->direction != SOUTH) {
                        g->snake->direction=NORTH;
                    }
                    break;
                case SDL_SCANCODE_D:
                    if(g->snake->direction != WEST) {
                        g->snake->direction=EAST;
                    }
                    break;
                case SDL_SCANCODE_S:
                    if(g->snake->direction != NORTH) {
                        g->snake->direction=SOUTH;
                    }
                    break;
                case SDL_SCANCODE_A:
                    if(g->snake->direction != EAST) {
                        g->snake->direction=WEST;
                    }
                    break;

                case SDL_SCANCODE_SPACE:
                    g->snake->eating = true;
                    g->snake->length+=1;
                    break;
                default:
                    break;
            }
        default:
          break;
        }
    }
}


void game_run(struct Game *g) {
    while(g->is_running) {
        uint64_t start = SDL_GetPerformanceCounter();
        game_events(g);
        snake_draw(g);
        renderer_update(g);

        uint64_t end = SDL_GetPerformanceCounter();
        double elapsed = (double)(end-start) / (double)SDL_GetPerformanceFrequency();
        if (elapsed < (double)TARGET_FPS) {
            SDL_Delay((TARGET_FPS - elapsed) * 1000);
            SDL_Delay(GAME_DELAY_MS);
        }
    }
}
