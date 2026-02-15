#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chip8.h"

void chip8_init(Chip8 *cpu)
{
    memset(cpu, 0, sizeof(Chip8));
    cpu->pc = 0x200;
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
