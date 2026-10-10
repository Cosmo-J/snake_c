#include "rendering.h"

bool draw_pixel(struct Game *g, uint32_t x, uint32_t y, uint8_t colour) {
    if(x >= CANVAS_WIDTH || y >= CANVAS_HEIGHT){
        fprintf(stderr, "Error updating framebuffer, accessed out of range.\n");
        return false;
    }

    uint32_t target_pixel = (y * CANVAS_WIDTH) + x;
    g->framebuffer[target_pixel] = colour;
    return true;
}


bool renderer_update(struct Game *g) {
    for(int x = 0; x<CANVAS_WIDTH; x++) {
        draw_pixel(g,x,0,255);
        draw_pixel(g,x,CANVAS_HEIGHT-1,255);
    }
    for(int y = 0; y<CANVAS_HEIGHT; y++) {
        draw_pixel(g,0,y,255);
        draw_pixel(g,CANVAS_WIDTH-1,y,255);
    }

    if (!SDL_UpdateTexture(g->canvas,NULL, g->framebuffer, CANVAS_PITCH)) {
        fprintf(stderr,"Error updating SDL Texture: %s\n",SDL_GetError());
        return false;
    }

    if (!SDL_RenderClear(g->renderer)) {
        fprintf(stderr,"Error clearing SDL Renderer: %s\n",SDL_GetError());
        return false;
    }

    if (!SDL_RenderTexture(g->renderer, g->canvas, NULL, NULL)) {
        fprintf(stderr,"Error SDL Rendering Texture: %s\n",SDL_GetError());
        return false;
    }

    if (!SDL_RenderPresent(g->renderer)) {
        fprintf(stderr,"Error presenting to SDL Renderer: %s\n",SDL_GetError());
        return false;
    }

    return true;
}
