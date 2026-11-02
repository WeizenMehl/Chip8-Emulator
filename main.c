# include <stdio.h>
# include <stdlib.h>

unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


// to represent the 35 opcodes, each one has 2 Bytes which is how long an unassinged short is
unsigned short opcode;

// to represent the 4kiB of memory
unsigned char memory[4096];
    
// to represent the 15 8-bit general purpose registers each named V0,V1,.., VE.
unsigned char V[16]; 

// to represent the Index register I and pc
// from the values 0x000 to 0xFFF
unsigned short I;
unsigned short pc;

// to represent the graphics of the Chip8
unsigned char gfx[64 * 32];

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

// to represent the Hex-Based Keypad(0x0-0xF)
unsigned char key[16];


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

    decodeOpcode();
}

void decodeOpcode(){
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x0000:
                    for(size_t i = 0; i < sizeof  gfx;i++)  gfx[i] = 0;
                    pc + 2;
                break;
                case 0x000E:
                    pc = stack[sp]; //this probably isnt correct
                    pc +2;
                break;
            }
        break;

        case 0x1000:
            pc = opcode & 0x0FFF;
        break;

        case 0x2000:
            stack[sp] = pc;
            ++pc;
            pc = opcode & 0x0FFF;
        break;

        case 0x3000:
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                pc + 4;
            }
            else{pc +2;}
        break;

        case 0x4000:
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                pc + 4;
            }
            else{pc+2;}
        break;

        case 0x5000:
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
                pc +4;
            }
            else{pc+2;}
        break;

        case 0x6000:
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc +2;
        break;

        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc +2;
        break;

        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc +2;
                break;

                case 0x0001:
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc +2;
                break;

                case 0x0002:
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc +2;
                break;

                case 0x0003:
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc +2;
                break;

                case 0x0004:
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])){
                        V[0xF] = 1;
                    }
                    else{
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;          
                break;

                case 0x0005:
                    if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]){
                        V[0xF] = 0;
                    }
                    else{
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc +2;
                break;

                case 0x0006:
                    V[(opcode & 0x00F0) >> 4] = V[(opcode & 0x0F00) >> 8] & 0x0001; // this is probably wrong
                    V[(opcode & 0x0F00) >> 8] >>=1;
                    pc +2;
                break;

                case 0x0007:
                    if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]){
                        V[0xF] = 0;
                    }
                    else{
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
                    pc +2;
                break;

                case 0x000E:
                    V[(opcode & 0x00F0) >> 4] = V[(opcode & 0x0F00) >> 8] & 0x1000; // this is probably wrong
                    V[(opcode & 0x0F00) >> 8] <<=1;
                    pc +2;
                break;
            }

            case 0x9000:
                if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
                    pc+4;
                }
                else{
                    pc+2;
                }
            break;

            case 0xA000:
                I = opcode & 0x0FFF;
                pc +2;
            break;

            case 0xB000:
                pc = (opcode & 0x0FFF) + V[0x0];
            break;

            case 0xC000:
                V[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
                pc +2;
            break;

            case 0xD000:
                // Draws pixels will implement in the future
            break;
        break;
    }
}
