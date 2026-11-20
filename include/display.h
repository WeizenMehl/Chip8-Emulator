# include <SDL2/SDL.h>

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

# define WINDOW_WIDTH 64
# define WINDOW_HEIGHT 32

void updateDisplay();
void DrawOnDisplay(unsigned short x,unsigned short y,unsigned short z);

extern unsigned char gfx[WINDOW_WIDTH * WINDOW_HEIGHT];
extern bool updateDisplayFlag;
extern SDL_Renderer *renderer;
extern SDL_Window *window;


#endif