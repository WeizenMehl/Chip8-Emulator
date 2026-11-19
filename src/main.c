#define _POSIX_C_SOURCE 199309L

# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <SDL2/SDL.h> 
# include <stdbool.h>

# include "display.h"
# include "cpu.h"
# include "memory.h"
# include "keypad.h"

# define WINDOW_WIDTH 64
# define WINDOW_HEIGHT 34 

// to represent the 35 opcodes, each one has 2 Bytes which is how long an unassinged short is
unsigned short opcode;

int loadProgramm(const char *filename);


void initialize(){
     pc = 0x200;
     opcode = 0;
     I = 0;
     sp = 0;

    for(size_t i = 0; i < sizeof  gfx;i++)  gfx[i] = 0;
    for(size_t i = 0; i < sizeof  stack;i++)  stack[i] = 0;
    for(size_t i = 0; i < sizeof  V;i++)  V[i] = 0;
    for(size_t i = 0; i < sizeof  memory;i++)  memory[i] = 0;

    for(int i = 0; i < 80; ++i)
         memory[i] = chip8_fontset[i];	

     delay_timer = 0;
     sound_timer = 0;
}

int loadProgramm(const char *filename){
    FILE* programm = fopen(filename, "rb");
    if (!programm){
        perror("Couldnt open programm file");
        return 0;
    }

    fseek(programm, 0L, SEEK_END);

    long int buffer_size = ftell(programm);

    if(buffer_size > sizeof  memory - 0x200){
        perror("Programm is to big to load into memory");
        return 0;
    }

    rewind(programm);

    fread( memory + 0x200,buffer_size,1,programm);

    fclose(programm);
    return 1;
}

void emulateCycle(){
    opcode = memory[pc] << 8 | memory[pc+1];
    pc+=2;

    decodeOpcode(opcode);
}

int main(){
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH * 10, WINDOW_HEIGHT * 10, 0, &window, &renderer);
    struct timespec start, now;

    initialize();

    loadProgramm("TETRIS");

    clock_gettime(CLOCK_MONOTONIC,&start);

    for(;;){
        clock_gettime(CLOCK_MONOTONIC,&now);

        double elapsed = (start.tv_nsec - now.tv_nsec)/ 1e9;

        if(elapsed >= 1/500){
            emulateCycle();
        }

        if(delay_timer > 0 && elapsed >= 1/60) delay_timer--;
        if(sound_timer > 0 && elapsed >= 1/60){
            // gonna need to implement beep
            sound_timer--;
        }

        if(updateDisplayFlag){ updateDisplay();}

        getUserInput();
    }

    return 0;
}