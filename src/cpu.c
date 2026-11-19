# include <stdio.h>
# include <stdbool.h>

# include "display.h"
# include "cpu.h"
# include "memory.h"
# include "keypad.h"

    
// to represent the 15 8-bit general purpose registers each named V0,V1,.., VE.
unsigned char V[16]; 

// to represent the Index register I and pc
// from the values 0x000 to 0xFFF
unsigned short I;
unsigned short pc;

// eventho the Chip8 has no interups or hardware registers it
// has two timer registers that count at 60Hz
// when set to an value above zero they will count down
unsigned char delay_timer;
unsigned char sound_timer;

// because the Chip8 has opcodes that allow the programm to jump to a certain address
// or call an certain suproutine we need an stack to store the current location when one of those is performed
// to remeber which level of stack is used we neep to implent an stack pointer
unsigned short stack[16];
unsigned short sp;

void decodeOpcode(short opcode){
    unsigned int x = (opcode & 0x0F00)>>8;
    unsigned int y = (opcode & 0x00F0)>>4;
    printf("PC: 0x%03X | OPCODE: 0x%04X\n", pc, opcode);
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x0000:
                    for(size_t i = 0; i < sizeof  gfx;i++)  gfx[i] = 0;
                    updateDisplayFlag = true;
                    pc  += 2;
                break;
                case 0x000E:
                    sp--;
                    pc = stack[sp]; //this probably isnt correct
                    pc+=2;
                break;
            }
        break;

        case 0x1000:
            pc = opcode & 0x0FFF;
        break;

        case 0x2000:
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
        break;

        case 0x3000:
            if(V[x] == (opcode & 0x00FF)){
                pc  += 4;
            }
            else{pc  +=2;}
        break;

        case 0x4000:
            if(V[x] != (opcode & 0x00FF)){
                pc  += 4;
            }
            else{pc +=2;}
        break;

        case 0x5000:
            if(V[x] == V[y]){
                pc  +=4;
            }
            else{pc +=2;}
        break;

        case 0x6000:
            V[x] = opcode & 0x00FF;
            pc  +=2;
        break;

        case 0x7000:
            V[x] += opcode & 0x00FF;
            pc  +=2;
        break;

        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:
                    V[x] = V[y];
                    pc  +=2;
                break;

                case 0x0001:
                    V[x] |= V[y];
                    pc  +=2;
                break;

                case 0x0002:
                    V[x] &= V[y];
                    pc  +=2;
                break;

                case 0x0003:
                    V[x] ^= V[y];
                    pc  +=2;
                break;

                case 0x0004:
                    if(V[y] > (0xFF - V[x])){
                        V[0xF] = 1;
                    }
                    else{
                        V[0xF] = 0;
                    }
                    V[x] += V[y];
                    pc += 2;          
                break;

                case 0x0005:
                    if(V[y] > V[x]){
                        V[0xF] = 0;
                    }
                    else{
                        V[0xF] = 1;
                    }
                    V[x] -= V[y];
                    pc  +=2;
                break;

                case 0x0006:
                    V[0xF] = V[x] & 0x0001; // this is probably wrong
                    V[x] >>=1;
                    pc  +=2;
                break;

                case 0x0007:
                    if(V[x] > V[y]){
                        V[0xF] = 0;
                    }
                    else{
                        V[0xF] = 1;
                    }
                    V[x] - V[y];
                    pc  +=2;
                break;

                case 0x000E:
                    V[0xF] = V[x] & 0x80;
                    V[x] <<=1;
                    pc  +=2;
                break;
            }

        case 0x9000:
            if(V[x] != V[y]){
                pc +=4;
            }
            else{
                pc +=2;
            }
        break;

        case 0xA000:
            I = opcode & 0x0FFF;
            pc +=2;
        break;

        case 0xB000:
            pc = (opcode & 0x0FFF) + V[0x0];
        break;

        case 0xC000:
            V[x] = rand() & (opcode & 0x00FF);
            pc += 2;
        break;

        case 0xD000:                
            unsigned short z = V[opcode & 0x000F];

            DrawOnDisplay(x,y,z);
            pc += 2;
        break;

        case 0xE000:
            switch(opcode & 0x00F){
                case 0x000E:
                    if(key[V[(opcode & 0x0F00)>>8]] != false){
                        pc += 4;
                    }
                    else{
                        pc += 2;
                    }
                break;

                case 0x0001:
                    if(key[V[(opcode & 0x0F00)>>8]] == false){
                        pc  +=4;
                    }
                    else{
                        pc +=2;
                    }
                break;
            }
        break;

        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007:
                    V[x] = delay_timer;
                    pc +=2;
                break;
                
                case 0x000A:
                    //This is wrong and I need to modify this further
                    for(int i;i<=16;i++){
                        if(key[i] != false){
                            V[x] = key[i];
                            pc +=2;
                        }
                    }
                break;

                case 0x0015:
                    delay_timer = V[x];
                    pc +=2;
                break;

                case 0x0018:
                    sound_timer = V[x];
                    pc +=2;
                break;

                case 0x001E:
                    I += V[x];
                    pc+=2;
                break;

                case 0x0029:
                    I = V[x] * 5; //was autocompleted
                    pc+=2;
                break;

                case 0x0033:
                    unsigned short x = (opcode & 0x0F00);
                    memory[I] = x/100;
                    memory[I+1] = (x)/10%10;
                    memory[I+2] = x%10;
                    pc+=2;
                break;

                case 0x0055:
                    for(int i = 0;i<=x;i++){
                        memory[I+i] = V[i]; 
                    }
                    pc+=2;
                break;

                case 0x0065:
                    for(int i = 0;i<=x;i++){
                        V[i] = memory[I+1]; 
                    }
                    pc+=2;
                break;
            }
        break;
    }
}