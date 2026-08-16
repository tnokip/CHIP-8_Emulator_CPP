#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;


const uint Starting_Address = 0x200;    //rom starting address
const uint Fontset_Starting_Address = 0x50;    //fontset starting address
const uint Fontset_Size = 80;
const int width {64};
const int height {32};
const float scale {20};           // display scale multiplier
extern bool drawflag ;
//extern bool clearflag ;
//extern bool dxynflag ;
extern std::string filename;

class chip8
{   
    U8 registers[16] {};    //V0-VF
    U8 memory[4096]{};      // 4k bytes of memory
    U16 index{};            //  Index registers
    U16 pc{};               //  program couneters
    U8 sp{};                //  stack pointer
    U16 stack[16]{};        //  stack
    U8 soundtimer{};
    U8 delaytimer{};
    U16 keypad[16]{};       //16 hex inputs
    U32 display[width*height]{};   // 64*32 display
    U16 opcode{};           //16 bit opcode (2 byte)
    std::mt19937 generator;
    std::uniform_int_distribution<U8> dist;
public:
    chip8();
    void LoadROM();
    void setTimer();
    uint randGen();
    void setKey(int in, bool pressed);
    const U16 getKey(int a);
    void cycle();
    const U32* getDisplay() const { return display; }
};



