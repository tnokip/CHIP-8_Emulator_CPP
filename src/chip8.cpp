#include "../include/chip8.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>



bool drawflag = false;
bool dxynflag = false;
std::string filename = "c8 roms/TETRIS";

U8 fontset[Fontset_Size] =
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


chip8::chip8() 
{
    pc = Starting_Address;    
    opcode = 0;
   //display[width*height] = {0}; 
   memset(display, 0, sizeof(display));
   //keypad[16] = {0} ; 
   memset(keypad,0, sizeof(keypad)); 
   memset(memory,0,sizeof(memory));
   memset(stack,0,sizeof(stack));
   memset(registers,0,sizeof(registers));

    soundtimer = 0;
    delaytimer = 0; 
    sp = 0;

   for(uint i = 0; i< Fontset_Size; i++)
        {
            memory[Fontset_Starting_Address + i] = fontset[i];
        }
    
   generator.seed(std::random_device{}());
   dist = std::uniform_int_distribution<U8>(0,255);
    
}

uint chip8::randGen()
{
    return dist(generator);
}


void chip8::LoadROM()
{
    std::ifstream  file(filename, std::ios::ate | std::ios::binary);
    
    if(file.is_open())
    {
        std::streampos size = file.tellg();
        char *buffer = new char[size];

        file.seekg(0,std::ios::beg);
        file.read(buffer,size);
        file.close();

        for(uint i = 0; i< size; i++)
        {
            memory[Starting_Address + i] = buffer[i];
        }

        delete[] buffer;

    }
}

void chip8::setTimer()
{
    if(soundtimer > 0)soundtimer--;
    if(delaytimer > 0)delaytimer--;
    
}

void chip8::setKey(int in, bool pressed) 
{
    if (in >= 0 && in < 16) 
    {
        keypad[in] = (pressed) ? 1 : 0;
    }
}
   
const U16 chip8::getKey(int a)
{
    return keypad[a];
}

void chip8::cycle()
{

    //fetch
    opcode = (memory[pc] << 8) | (memory[pc + 1]) ;
    
switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x00FF)
            {
                    
                case 0x00E0:    // 0x00E0 - clear screen
                    memset(display,0,sizeof(display));
                    drawflag = true;
                    pc += 2;
                    break;

                case 0x00EE:    // 0x00EE - return from subroutine
                    sp--;
                    pc = stack[sp];
                    pc += 2;
                    break;

                
                default:
                    std::cout<<"Unknown opcode: " << std::hex << opcode << "\n";
                    pc += 2;
                    break;
            }
            break;

        case 0x1000:    // 0x1NNN - jump to NNN
            pc = opcode & 0x0FFF;
            break;
        
        case 0x2000:    // 0x2NNN - call subroutine
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
            break;

        case 0x3000:    // 0x3XNN - skips next instruction if 
                        // Vx == NN
            if(registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            {
                pc += 4;
            }
            else pc += 2;
            break;

        case 0x4000:    // 0x4XNN - skips next instruction if 
                        // Vx != NN
        
            if(registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            {
                pc += 4;
            }
            else pc += 2;
            break;

        case 0x5000:    // 0x5XY0 - skips next instruction 
                        // if Vx == Vy
            if(registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4])
            {
                pc += 4;
            }
            else pc += 2;
            break;

        case 0x6000:    // 0x6XNN - set Vx to NN
            registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) ;
            pc += 2;
            break;

        case 0x7000:    // 0x7XNN - add NN to Vx
            registers[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF) ;
            pc +=2;
            break;

        case 0x8000:    // 0x8XY0 to 8XYE
            switch(opcode & 0x000F)
            {
                case 0x0000:    // 0x8XY0 set Vx value to Vy
                registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;

                case 0x0001:    // 0x8XY1 set Vx value to Vx OR Vy
                registers[(opcode & 0x0F00) >> 8] |= registers[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
                
                case 0x0002:    // 0x8XY2 set Vx value to Vx AND Vy
                registers[(opcode & 0x0F00) >> 8] &= registers[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
                
                case 0x0003:    // 0x8XY3 set Vx value to Vx XOR Vy
                registers[(opcode & 0x0F00) >> 8] ^= registers[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
                
                
                case 0x0004:    // 0x8XY4 set Vx value to Vx + Vy and Vf to carry
                if(registers[(opcode & 0x00F0) >> 4]  > (0xFF - registers[(opcode & 0x0F00) >> 8]))
                 { registers[0xF] = 1; }
                else registers[0xF] = 0;
                registers[(opcode & 0x0F00) >> 8] += registers[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;

                case 0x0005:    // 0x8XY5 set Vx value to Vx - Vy and Vf to NOT borrow
                                // i.e. when there is borrow Vf = 0, and vice versa
                registers[(opcode & 0x0F00) >> 8] -= registers[(opcode & 0x00F0) >> 4];
                if(registers[(opcode & 0x00F0) >> 4]  > (registers[(opcode & 0x0F00) >> 8]))
                 { registers[0xF] = 0; }
                else registers[0xF] = 1;
                pc += 2;
                break;

                case 0x0006:    // 0x8XY6 - set Vf to least significant bit of Vx
                                // then Vx set to Vx/2 i.e. shift right by 1
                registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0x1;
                registers[(opcode & 0x0F00) >> 8] >>= 1;
                pc += 2;
                break;

                case 0x0007:    // 0x8XY7 - Vx = Vy -Vx     Vf = NOT borrow
                if(registers[(opcode & 0x00F0) >> 4] > registers[(opcode & 0x0F00) >> 8])
                {
                    registers[0xF] = 1;
                }
                else registers[0xF] = 0;
                registers[(opcode & 0x0F00) >> 8] = 
                registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8];
                pc += 2;
                break;

                case 0x000E:    // 0x8XYE - Vf =  most significant bit of Vx
                                // Vx * 2 i.e. Vx left shifted
                registers[0xF] = (registers[(opcode & 0x0F00) >> 8] >> 7) & 0x1;
                registers[(opcode & 0x0F00) >> 8] <<=1;
                pc += 2;
                break;

            }   
            break;

        case 0x9000:    // 0x9XY0 - skip next instruction if Vx != Vy
                if(registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4])
                    pc += 4;
                else pc += 2;
                break;



        case 0xA000:    // 0xANNN - set index register to NNN
            index = (0x0FFF & opcode);
            pc += 2;
            break;
        
        case 0xB000:    // 0xBNNN - jump to V0 + NNN
            pc = registers[0x0] + ((opcode & 0x0FFF));
            break;

        case 0xC000:    // 0x CXNN - Vx = random AND NN
                registers[(opcode & 0x0F00) >> 8] = randGen() & (opcode & 0x00FF);
                pc += 2;
                break;


        case 0xD000:    // 0xDXYN - draw from x , 8 pixels wide 
                        // and from y n pixels tall
         {
            unsigned short x = registers[(opcode & 0x0F00) >> 8];
            unsigned short y = registers[(opcode & 0x00F0) >> 4];
            unsigned short n = opcode & 0x000F ;
            registers[0xF] = 0;

            for(int i = 0;i<n; i++ ) 
            {
                unsigned short sprite = memory[index + i];
                for(int j = 0; j < 8; j++)
                {
                    if(sprite & (0x80 >> j))
                    {
                        //int px = (j + x)% width;
                        //nt py = (i + y)% height;
                        //int pos = py * width + px;
                        if(display[(x+j) + ((y+i)*64)] == 1)
                        {
                            registers[0xF] = 1;
                        }

                        display[(x+j) + ((y+i)*64)] ^= 1;
                    }
                }
            }
            drawflag = true;
            pc += 2;
       }
            break;

        case 0xE000:   
            
            switch(opcode & 0x00FF)
            {
                case 0x009E:// 0xEX9E skip next instruction if key with the 
                            // value of Vx is pressed
               if(getKey(registers[(opcode & 0x0F00)>>8]) == 1)pc += 4;
               else pc += 2;
               break;

                case 0x00A1:    //0xEXA1 - skip next instruction if key stored
                                //in Vx isnt pressed
               if(getKey(registers[(opcode & 0x0F00)>>8]) == 0)pc += 4;
               else pc += 2;
               break;
            
                default:
                    std::cout<<"Unknown opcode: " << std::hex << opcode << "\n";
                    pc += 2;
                    break;
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:    //0xFX07 set Vx to the value of delay timer
                    registers[(opcode & 0x0F00)>>8] = delaytimer;
                    pc += 2;
                    break;

                case 0x000A:    // 0xFX0A store awaited key press in x
                    {   
                    bool press = false;
                        for(int b = 0; b<16 ; b++)
                        {
                            if(getKey(b) != 0)
                            {
                                registers[(opcode & 0x0F00)>>8] = b;
                                press = true;
                            }
                        }

                        if(!press) return;
                        pc += 2;
                    }  
                    break;

                case 0x0015: //0xFX15  sets delay timer to Vx
                        delaytimer = registers[(opcode & 0x0F00)>>8];
                        pc += 2;
                        break;

                case 0x0018:    //0xFX18 sets sound timer to Vx
                        soundtimer = registers[(opcode & 0x0F00) >> 8];
                        pc += 2;
                        break;


                case 0x001E:    //0xFX1E Vf =1 when I+Vx > 0xFFF, and vice versa
                        if((index + registers[(opcode & 0x0F00)>>8]) > 0xFFF)
                        {   registers[0xF] = 1; }
                        else registers[0xF] = 0;
                        index += registers[(opcode & 0x0F00) >> 8];
                        pc += 2;
                        break;

                case 0x0029:    //0xFX29 - set index to location of sprite 
                                //corresponding to the value of Vx
                    index = 0x50 + (registers[(opcode & 0x0F00) >> 8] * 0x5);
                    pc += 2;
                    break;

                // 0xFX33 - Stores the Binary-coded decimal representation of VX
                // at the addresses I, I plus 1, and I plus 2
                case 0x0033:    
                    memory[index]     = registers[(opcode & 0x0F00) >> 8] / 100;
                    memory[index + 1] = (registers[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[index + 2] = registers[(opcode & 0x0F00) >> 8] % 10;
                    pc += 2;
                    break;

                // FX55 - Stores V0 to VX in memory starting at address I
                case 0x0055:
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                        memory[index + i] = registers[i];

                    // On the original interpreter, when the
                    // operation is done, I = I + X + 1.
                    index += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                    break;

                case 0x0065:
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                        registers[i] = memory[index + i];

                    // On the original interpreter,
                    // when the operation is done, I = I + X + 1.
                    index += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                    break;
                    
                default:
                    std::cout<<"Unknown opcode: " << std::hex << opcode << "\n";
                    pc += 2;
                    break;
            }

    } 
    //timers
    //soundtimer++;
    //delaytimer++;
    //setTimer();
}

