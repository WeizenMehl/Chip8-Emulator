# include <stdbool.h>
# include <SDL2/SDL.h> 

# include "keypad.h"

// to represent the Hex-Based Keypad(0x0-0xF)
unsigned char key[16];

void getUserInput(){
    SDL_Event event;

    while(SDL_PollEvent( &event)){
        if(event.type == SDL_QUIT)
            exit(0);
        
        else if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN){
            bool state = (event.type == SDL_KEYDOWN);

            keyPressed(event.key.keysym.sym, state);
        }
    }
}

void keyPressed(SDL_KeyCode c,bool state){
    switch(c){
        case '1':key[0x1] = state;break;
        case '2':key[0x2] = state;break;
        case '3':key[0x3] = state;break;
        case '4':key[0xC] = state;break;
        case 'q':key[0x4] = state;break;
        case 'w':key[0x5] = state;break;
        case 'e':key[0x6] = state;break;
        case 'r':key[0xD] = state;break;
        case 'a':key[0x7] = state;break;
        case 's':key[0x8] = state;break;
        case 'd':key[0x9] = state;break;
        case 'f':key[0xE] = state;break;
        case 'z':key[0xA] = state;break;
        case 'x':key[0x0] = state;break;
        case 'c':key[0xB] = state;break;
        case 'v':key[0xF] = state;break;
    }
}