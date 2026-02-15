#include <stdio.h>
// #include "../lowlevel/mychip8/include/chip8.h"
#include "chip8.h"
void print_font_visual(Chip8 *cpu, int digit)
{
    printf("Font for digit %X:\n", digit);
    for (int i = 0; i < 5; i++)
    {
        uint8_t byte = cpu->memory[digit * 5 + i];
        for (int bit = 7; bit >= 4; bit--)
        { // Only use top 4 bits
            printf("%c", (byte & (1 << bit)) ? '#' : '.');
        }
        printf("\n");
    }
}

int main()
{
    Chip8 cpu;
    chip8_init(&cpu);
    printf("Testing fontset loading: \n");
    printf("First character (0) at 0x000\n");
    // for (int i = 0; i)

    if (chip8_load_rom(&cpu, "ibm_logo.ch8"))
    {
        printf("\nError loading\n");
        return 1;
    }

    printf("\nFirst 10 ROM bytes at 0x200");
    for (int i = 0; i < 10; i++)
    {
        printf("Opcode byte is %d: %02X\n", i, cpu.memory[0x200 + i]);
    }
    return 0;
}