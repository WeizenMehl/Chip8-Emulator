#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

void decodeOpcode(unsigned short);

extern unsigned char V[16]; 
extern unsigned short I;
extern unsigned short pc;
extern unsigned char delay_timer;
extern unsigned char sound_timer;
extern unsigned short stack[16];
extern unsigned short sp;

#endif