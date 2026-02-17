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

    srand(time(NULL));

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

        uint8_t keymap[16] = {
            KEY_X,     // 0
            KEY_ONE,   // 1
            KEY_TWO,   // 2
            KEY_THREE, // 3
            KEY_Q,     // 4
            KEY_W,     // 5
            KEY_E,     // 6
            KEY_A,     // 7
            KEY_S,     // 8
            KEY_D,     // 9
            KEY_Z,     // A
            KEY_C,     // B
            KEY_FOUR,  // C
            KEY_R,     // D
            KEY_F,     // E
            KEY_V      // F
        };

        for (int i = 0; i < 16; i++)
        {
            cpu.keypad[i] = IsKeyDown(keymap[i]);
        }

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

    return 0;
}