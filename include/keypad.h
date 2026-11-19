#ifndef KEYPAD_H_INCLUDED
#define KEYPAD_H_INCLUDED

void getUserInput();
void keyPressed(char c,bool state);
extern unsigned char key[16];

#endif