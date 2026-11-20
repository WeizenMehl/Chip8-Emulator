#ifndef KEYPAD_H_INCLUDED
#define KEYPAD_H_INCLUDED

void getUserInput();
void keyPressed(SDL_KeyCode c,bool state);
extern unsigned char key[16];

#endif