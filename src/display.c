# include <stdio.h>
# include <SDL2/SDL.h>
# include <stdbool.h>

# include "display.h"
# include "memory.h"
# include "cpu.h"

# define WINDOW_WIDTH 64
# define WINDOW_HEIGHT 34

// to represent the graphics of the Chip8
unsigned char gfx[WINDOW_WIDTH * WINDOW_HEIGHT];

bool updateDisplayFlag = false;

SDL_Renderer *renderer;
SDL_Window *window;


//have to figure out better name for this function
void DrawOnDisplay(unsigned short x, unsigned short y, unsigned short z){
    unsigned short pixel;
    V[0xF] = 0;
    for(int row = 0; row < z; row++){
        pixel = memory[I + row];
        for(int column = 0; column < 8; column++){
            if((pixel & (0x80 >> column)) != 0){
                int px = (x + column) % WINDOW_WIDTH;
                int py = (y + row) % WINDOW_HEIGHT;
                int index = py * WINDOW_WIDTH + px;
                if(gfx[index] == 1){
                    V[0xF] = 1;
                }

                gfx[index] ^= 1;
            }
        }
    }

    updateDisplayFlag = true;
}
void updateDisplay(){
    for(int row = 0;row<WINDOW_HEIGHT;row++){
        for(int column = 0;column<WINDOW_WIDTH;column++){
            int i = row * WINDOW_WIDTH + column;
            if(gfx[i] == 1){
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_Rect pixel = {column*10,row*10,10,10};
                SDL_RenderFillRect(renderer, &pixel);    
            }
            else{
                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                SDL_Rect pixel = {column*10,row*10,10,10};
                SDL_RenderFillRect(renderer, &pixel);
            }
            i++;
        }
    }
    SDL_RenderPresent(renderer);
    updateDisplayFlag = false;
}