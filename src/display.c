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
void DrawOnDisplay(unsigned short x,unsigned short y,unsigned short z){
    unsigned short pixel;
    V[0xF] = 0;
    for(int row = 0;row <= z;row++){
        pixel = memory[I + row];
        for(int column = 0;column <= 8;column++){
            if(((pixel & 0x80)>>row) != 0){
                if(gfx[(x + column + y + row) * WINDOW_WIDTH] == 1){
                    V[0xF] = 1;
                    gfx[(x + column + y + row) * WINDOW_WIDTH] = 0;
                }
                else{
                    gfx[(x + column + y + row) * WINDOW_WIDTH] = 1;
                }
            }
        }
    }
    updateDisplayFlag = true;
}

void updateDisplay(){
    for(int row = 0;row<=WINDOW_WIDTH;row++){
        for(int column = 0;column<=WINDOW_HEIGHT;column++){
            if(gfx[row,column] == 1){
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderDrawPoint(renderer, row, column);    
            }
            else{
                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                SDL_RenderDrawPoint(renderer, row, column); 
            }
        }
    }
    SDL_RenderPresent(renderer);
    updateDisplayFlag = false;
}