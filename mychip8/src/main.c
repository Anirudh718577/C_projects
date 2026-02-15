#include <stdio.h>
// #include "../lowlevel/mychip8/include/chip8.h"
#include "chip8.h"
#include "raylib.h"

#define SCALE 10
#define SCREEN_WIDTH (64 * SCALE)
#define SCREEN_HEIGHT (32 * SCALE)

#define CYCLE_SPEED 10

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <rom_file> \n", argv[0]);
        return 1;
    }

    Chip8 cpu;
    chip8_init(&cpu);

    if (chip8_load_rom(&cpu, argv[1]))
    {
        printf("\nError loading\n");
        return 1;
    }

    // InitWindow(640, 320, "Chip8 Emulator");
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chip8 Emulator");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        for (int i = 0; i < CYCLE_SPEED; i++)
        {
            chip8_cycle(&cpu);
        }

        if (cpu.delay_timer > 0)
            cpu.delay_timer--;
        if (cpu.sound_timer > 0)
            cpu.sound_timer--;

        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < 32; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                if (cpu.display[y * 64 + x])
                {
                    DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, WHITE);
                }
            }
        }

        EndDrawing();
    }
    CloseWindow();

    // printf("%x\n", cpu.memory[cpu.pc] << 8);

    // printf("\nFirst 10 ROM bytes at 0x200\n");
    // for (int i = 0; i < 10; i++)
    // {
    //     printf("Opcode byte is %d: %02X\n", i, cpu.memory[0x200 + i]);
    // }
    // chip8_cycle(&cpu);

    return 0;
}