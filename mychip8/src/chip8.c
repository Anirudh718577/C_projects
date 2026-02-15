#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chip8.h"

// fontset taken from cowgod's technical reference
static const uint8_t chip8_fontset[80] = {
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

void chip8_init(Chip8 *cpu)
{
    memset(cpu, 0, sizeof(Chip8));
    cpu->pc = 0x200;

    memcpy(cpu->memory, chip8_fontset, sizeof(chip8_fontset));
}

int chip8_load_rom(Chip8 *cpu, const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        return -1;
    }

    fread(&cpu->memory[0x200], 1, sizeof(cpu->memory) - 0x200, fp); // Load everything into main memory from the starting 0x200
    fclose(fp);
    return 0;
}

void chip8_cycle(Chip8 *cpu)
{
    // consists of three tasks fetch decode execute

    // fetching opcode for 2 bytes

    uint16_t opcode = (cpu->memory[cpu->pc] << 8);
    opcode = opcode | (cpu->memory[cpu->pc + 1]);

    cpu->pc += 2; // move to the next two bytes

    uint8_t X = (opcode & 0xF00) >> 8;  // Second Nibble
    uint8_t Y = (opcode & 0x00F0) >> 4; // Third Nibble
    uint8_t N = opcode & 0x000F;        // Fourth Nibble
    uint8_t NN = opcode & 0x00FF;       // Second Byte
    uint16_t NNN = opcode & 0x0FFF;     // 12 bit immediate address

    // printf("%x", opcode);

    switch (opcode & 0xF000)
    {
    case 0x000:
        switch (NN)
        {
        case 0xE0:
            memset(cpu->display, 0, sizeof(cpu->display));
            printf("Clearing Screen");
            break;
        }
        break;
    case 0x1000:
        cpu->pc = NNN;
        break;
    case 0x6000:
        cpu->V[X] = NN;
        break;
    case 0x7000:
        cpu->V[X] += NN;
        break;
    case 0xA000:
        cpu->I = NNN;
        break;
    case 0xD000:
        uint8_t x_pos = cpu->V[X] % 64;
        uint8_t y_pos = cpu->V[Y] % 32;
        cpu->V[0xF] = 0;
        for (uint8_t row = 0; row < N; row++)
        {
            uint8_t sprite_byte = cpu->memory[cpu->I + row];
            for (uint8_t col = 0; col < 8; col++)
            {
                uint8_t sprite_pixel = sprite_byte & (0x80 >> col);
                uint8_t *screen_pixel = &cpu->display[(y_pos + row) * 64 + (x_pos + col)];

                if (sprite_pixel)
                {
                    if (*screen_pixel == 1)
                        cpu->V[0xF] = 1;
                    *screen_pixel ^= 1;
                }
            }
        }
        break;
    }
}