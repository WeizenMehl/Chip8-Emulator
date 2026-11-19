#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

# define WINDOW_WIDTH 64
# define WINDOW_HEIGHT 34

void updateDisplay();
void DrawOnDisplay(unsigned short x,unsigned short y,unsigned short z);

#endif

extern unsigned char gfx[WINDOW_WIDTH * WINDOW_HEIGHT];
extern updateDisplayFlag = false;
extern SDL_Renderer *renderer;
extern SDL_Window *window;
