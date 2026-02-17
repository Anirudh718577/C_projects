#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chip8.h"
#include <stdbool.h>
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

    uint8_t X = (opcode & 0x0F00) >> 8; // Second Nibble
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
        case 0xE0: // 00E0 -> Clear display
            memset(cpu->display, 0, sizeof(cpu->display));
            break;
        case 0xEE: // 00EE - RET -> Return from a subroutine.
            cpu->sp--;
            cpu->pc = cpu->stack[cpu->sp];
            break;
        default:
            printf("Unknown opcode : 0x%04X\n", opcode);
            break;
        }
        break;

    case 0x1000: // 1NNN - Jump to location NNN
        cpu->pc = NNN;
        break;

    case 0x2000: // 2NNN - CALL subroutine at NNN
        cpu->stack[cpu->sp] = cpu->pc;
        cpu->sp++;
        cpu->pc = NNN;
        break;

    case 0x3000: // 3XNN - skip next instruction if VX = NN
        if (cpu->V[X] == NN)
        {
            cpu->pc += 2;
        }
        break;

    case 0x4000: // 4XNN - Skip next instruction if VX != NN
        if (cpu->V[X] != NN)
        {
            cpu->pc += 2;
        }
        break;

    case 0x5000: // 5XY0 - Skip next instruction if VX == VY
        if (cpu->V[X] == cpu->V[Y])
        {
            cpu->pc += 2;
        }
        break;

    case 0x6000: // 6XNN - Puts the value NN into register VX
        cpu->V[X] = NN;
        break;

    case 0x7000: // 7XNN - Adds the value NN to the value of register VX, then stores the result in VX
        cpu->V[X] += NN;
        break;

    case 0x8000:
        switch (N)
        {
        case 0x0:
            cpu->V[X] = cpu->V[Y];
            break;
        case 0x1:
            cpu->V[X] = cpu->V[X] | cpu->V[Y];
            break;
        case 0x2:
            cpu->V[X] = cpu->V[X] & cpu->V[Y];
            break;
        case 0x3:
            cpu->V[X] = cpu->V[X] ^ cpu->V[Y];
            break;
        case 0x4:
        {
            uint16_t sum = cpu->V[X] + cpu->V[Y];
            cpu->V[0xF] = (sum > 0xFF) ? 1 : 0;
            cpu->V[X] = sum & 0xFF;
            break;
        }
        case 0x5:
        {
            cpu->V[0xF] = (cpu->V[X] >= cpu->V[Y]) ? 1 : 0;
            cpu->V[X] = cpu->V[X] - cpu->V[Y];
            break;
        }
        case 0x6:
            cpu->V[0xF] = cpu->V[X] & 0x1;
            cpu->V[X] >>= 1;
            break;
        case 0x7:
            cpu->V[0xF] = (cpu->V[Y] >= cpu->V[X]) ? 1 : 0;
            cpu->V[X] = cpu->V[Y] - cpu->V[X];
            break;
        case 0xE:
            cpu->V[0xF] = (cpu->V[X] & 0x80) >> 7;
            cpu->V[X] <<= 1;
            break;
        default:
            break;
        }
        break;

    case 0xA000: // ANNN - Set I to NNN
        cpu->I = NNN;
        break;

    case 0xB000: // BNNN - Jump to NNN + V0
        cpu->pc = NNN + cpu->V[0];
        break;

    case 0xC000: // CXNN - Set VX to random number AND NN
        cpu->V[X] = ((rand() % 256) & NN);
        break;

    case 0xD000:
        uint8_t x_pos = cpu->V[X] % 64;
        uint8_t y_pos = cpu->V[Y] % 32;
        cpu->V[0xF] = 0;
        for (uint8_t row = 0; row < N; row++)
        {
            if ((y_pos + row) >= 32)
                break;
            uint8_t sprite_byte = cpu->memory[cpu->I + row];
            for (uint8_t col = 0; col < 8; col++)
            {
                if ((x_pos + col) >= 64)
                    break;

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

    case 0xE000:
        switch (NN)
        {
        case 0x9E: // EX9E - Skip if key VX is pressed
            if (cpu->keypad[cpu->V[X] & 0xF])
            {
                cpu->pc += 2;
            }
            break;
        case 0xA1: // EXA1 - Skip if key VX is not pressed
            if (!cpu->keypad[cpu->V[X] & 0xF])
            {
                cpu->pc += 2;
            }
            break;
        default:
            break;
        }
        break;
    case 0xF000:
        switch (NN)
        {
        case 0x07: // FX07 - Set VX to delay timer
            cpu->V[X] = cpu->delay_timer;
            break;
        case 0x0A:
            bool key_pressed = false;
            for (int i = 0; i < 16; i++)
            {
                if (cpu->keypad[i])
                {
                    cpu->V[X] = i;
                    key_pressed = true;
                    break;
                }
            }
            if (!key_pressed)
            {
                cpu->pc -= 2;
            }
            break;
        case 0x15: // FX15 - Set delay timer to VX
            cpu->delay_timer = cpu->V[X];
            break;
        case 0x18: // FX18 - Set sound timer to VX
            cpu->sound_timer = cpu->V[X];
            break;
        case 0x1E: // FX1E - Add VX to I
            cpu->I += cpu->V[X];
            // printf("Set I += V%X (now 0x%03X)\n", X, cpu->I);
            break;
        case 0x29:                  // FX29 - Set I to location of sprite for digit VX
            cpu->I = cpu->V[X] * 5; // Each font character is 5 bytes
            // printf("Set I = font[V%X] (0x%03X)\n", X, cpu->I);
            break;
        case 0x33: // FX33 - Store BCD representation of VX
            cpu->memory[cpu->I] = cpu->V[X] / 100;
            cpu->memory[cpu->I + 1] = (cpu->V[X] / 10) % 10;
            cpu->memory[cpu->I + 2] = cpu->V[X] % 10;
            // printf("Store BCD of V%X at I\n", X);
            break;
        case 0x55: // FX55 - Store V0 to VX in memory starting at I
            for (int i = 0; i <= X; i++)
            {
                cpu->memory[cpu->I + i] = cpu->V[i];
            }
            // printf("Store V0-V%X at I\n", X);
            break;
        case 0x65: // FX65 - Load V0 to VX from memory starting at I
            for (int i = 0; i <= X; i++)
            {
                cpu->V[i] = cpu->memory[cpu->I + i];
            }
            // printf("Load V0-V%X from I\n", X);
            break;
        default:
            // printf("Unknown opcode: 0x%04X\n", opcode);
            break;
        }
        break;
    default:
        break;
    }
}