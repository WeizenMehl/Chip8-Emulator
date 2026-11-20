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

    decodeOpcode(opcode);
}

int main(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH * 10, WINDOW_HEIGHT * 10, 0, &window, &renderer);
    SDL_StopTextInput();
    struct timespec start, now;

    SDL_AudioSpec spec;
    SDL_zero(spec);

    spec.freq = 44100;
    spec.format = AUDIO_F32SYS;
    spec.channels = 1;
    spec.samples = 4096;
 
    SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    SDL_PauseAudioDevice(device, 0); 

    float beep[44100 / 60];

    // I just combied that. I just wanted an beep sound that doesnt destroy my ears
    for (int i = 0; i < 44100 / 60; i++) 
        beep[i] = ((i * 440 * 2 / 44100) % 2) ? 0.2f : -0.2f;

    initialize();

    loadProgramm("TETRIS");

for(;;){
    clock_gettime(CLOCK_MONOTONIC, &start);

    for(int i = 0; i < 10; i++){
        emulateCycle();
    }

    if(delay_timer > 0) delay_timer--;
    if (sound_timer > 0) {
    if (SDL_GetQueuedAudioSize(device) < sizeof(beep) * 2)
        SDL_QueueAudio(device, beep, sizeof(beep));
    sound_timer--;
    } else {
        SDL_ClearQueuedAudio(device);       
    }

    if(updateDisplayFlag) updateDisplay();
    getUserInput();

    clock_gettime(CLOCK_MONOTONIC, &now);
    double elapsed = (now.tv_sec - start.tv_sec)
                   + (now.tv_nsec - start.tv_nsec) / 1e9;
    double remaining = 1.0/60.0 - elapsed;
    if(remaining > 0){
        SDL_Delay((Uint32)(remaining * 1000.0));
    }
}

    return 0;
}