#include <stdint.h>
#include <stddef.h>

#define STARTING_ADDRESS 0x200

typedef struct
{
    uint8_t memory[4096];
    uint8_t V[16];
    uint8_t I;
    uint8_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t display[64 * 32];
    uint8_t keypad[16];
} Chip8;

void chip8_init(Chip8 *cpu);
int chip8_load_rom(Chip8 *cpu, const char *filename);
void chip8_cycle(Chip8 *cpu);