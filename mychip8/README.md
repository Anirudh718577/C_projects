# CHIP-8 Emulator

A complete CHIP-8 emulator implementation in C with graphical output using Raylib. This emulator accurately recreates the behavior of the CHIP-8 virtual machine that was popular in the 1970s for running simple games and programs.

## Overview

CHIP-8 is an interpreted programming language developed in the mid-1970s for programming video games on 8-bit microcomputers. This emulator provides a modern way to run classic CHIP-8 programs on contemporary hardware.

## Features

- Full CHIP-8 instruction set implementation (35 opcodes)
- 64x32 pixel monochrome display with configurable scaling
- 16-key hexadecimal keypad input
- Sound timer support
- Configurable CPU cycle speed
- Includes collection of classic CHIP-8 ROMs (games, demos, and programs)

## Project Structure

``` bash
mychip8/
├── src/
│   ├── chip8.c          # Core CHIP-8 CPU emulation
│   └── main.c           # Main program and graphics/input handling
├── include/
│   └── chip8.h          # CHIP-8 data structures and API
├── obj/                 # Compiled object files (generated)
├── rom_files/           # Sample ROM files
├── chip8-roms/          # Extensive ROM collection
│   ├── games/           # Classic CHIP-8 games
│   ├── demos/           # Visual demonstrations
│   ├── programs/        # Utility programs
│   └── hires/           # High-resolution programs
├── Makefile             # Build configuration
└── README.md            # This file
```

## Prerequisites

Before building the emulator, ensure you have the following installed:

- **GCC** (GNU Compiler Collection)
- **Raylib** - Graphics library for display and input handling
- **Standard development libraries**: OpenGL, X11, pthread, math

On Ubuntu/Debian:

``` bash
sudo apt-get install gcc build-essential libraylib-dev libgl1-mesa-dev libx11-dev
```

## Building

To compile the emulator:

```bash
make
```

To clean build artifacts:

```bash
make clean
```

To rebuild from scratch:

```bash
make rebuild
```

## Usage

Run the emulator with a ROM file:

```bash
./chip8 <path-to-rom-file>
```

Example:

```bash
./chip8 rom_files/ibm_logo.ch8
./chip8 chip8-roms/games/Pong.ch8
./chip8 "chip8-roms/games/Breakout [Carmelo Cortez, 1979].ch8"
```

The emulator window will open displaying the program output. Close the window or press ESC to exit.

## Keyboard Layout

The original CHIP-8 used a 16-key hexadecimal keypad (0-F). This emulator maps those keys to your keyboard:

```bash
CHIP-8 Keypad:          Modern Keyboard:
+---+---+---+---+       +---+---+---+---+
| 1 | 2 | 3 | C |       | 1 | 2 | 3 | 4 |
+---+---+---+---+       +---+---+---+---+
| 4 | 5 | 6 | D |       | Q | W | E | R |
+---+---+---+---+       +---+---+---+---+
| 7 | 8 | 9 | E |       | A | S | D | F |
+---+---+---+---+       +---+---+---+---+
| A | 0 | B | F |       | Z | X | C | V |
+---+---+---+---+       +---+---+---+---+
```

## CHIP-8 Architecture

### Memory Layout (4KB)

| Address Range | Usage                                    |
|---------------|------------------------------------------|
| 0x000-0x1FF   | Reserved for interpreter (font set)      |
| 0x200-0xFFF   | Program memory (ROM loaded here)         |
| 0xEA0-0xEFF   | Call stack (internal use)                |
| 0xF00-0xFFF   | Display refresh (not used directly)      |

### Registers

- **V0-VF**: 16 general-purpose 8-bit registers
  - **VF**: Special flag register (carry, borrow, collision detection)
  - Avoid using VF for general storage as it's overwritten by arithmetic/draw operations
- **I**: 16-bit index register for memory addressing
- **PC**: Program counter (starts at 0x200)
- **SP**: Stack pointer (8-bit)

### Additional Components

- **Display**: 64x32 pixel monochrome framebuffer
- **Stack**: 16 levels for subroutine calls
- **Timers**:
  - Delay timer (60Hz countdown)
  - Sound timer (60Hz countdown, beeps when non-zero)
- **Keypad**: 16-key input (0x0-0xF)

### Display Configuration

The emulator scales the native 64x32 display by a factor of 10 (configurable in `main.c`):

- Window size: 640x320 pixels
- Refresh rate: 60 FPS
- CPU cycles per frame: 10 (adjustable via `CYCLE_SPEED`)

## Technical Details

### Instruction Set

CHIP-8 has 35 opcodes, all 2 bytes long. The instruction format uses:

- **X**: Second nibble, register identifier
- **Y**: Third nibble, register identifier  
- **N**: Fourth nibble, 4-bit immediate value
- **NN**: Second byte, 8-bit immediate value
- **NNN**: Last 12 bits, memory address

Example opcodes:

- `00E0`: Clear display
- `1NNN`: Jump to address NNN
- `6XNN`: Set VX to NN
- `7XNN`: Add NN to VX
- `DXYN`: Draw sprite at (VX, VY) with height N

### Implementation Notes

- Big-endian opcode fetching (high byte first)
- Sprites are XOR'd onto the display
- VF set on pixel collision during drawing
- PC increments by 2 for each instruction
- 16-level stack for subroutine nesting
- Fontset loaded at memory address 0x000

## Sample ROMs

The project includes an extensive collection of ROMs:

### Games

- Pong, Breakout, Space Invaders
- Tetris, Brix, Blitz
- Maze generators, puzzles
- And many more classics

### Demos

- Particle effects, starfield
- Sierpinski triangle
- Trip8 visual demo

### Programs

- IBM Logo (good test ROM)
- Utilities and tools

## Development

### Code Organization

- `chip8.c/h`: Pure emulation logic, no dependencies on graphics
- `main.c`: Graphics, input handling, and main loop using Raylib
- Modular design allows easy porting to different graphics backends

### Configuration

Key constants in `main.c`:

- `SCALE`: Display scaling factor (default: 10)
- `CYCLE_SPEED`: CPU instructions per frame (default: 10)
- `SetTargetFPS()`: Frame rate (default: 60)

## Debugging

### Memory Leak Detection

Run with Valgrind:

```bash
make valgrind
```

### Common Issues

1. **Black screen**: ROM may not be drawing or PC jumped to invalid memory
2. **Speed issues**: Adjust `CYCLE_SPEED` constant
3. **Input not working**: Check keymap configuration in `main.c`

## References

### Specifications & Guides

- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) - Comprehensive technical specification
- [Tobias V. Langhoff's Guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) - Excellent implementation guide
- [CHIP-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8) - History and overview
- [Columbia University Documentation](https://www.cs.columbia.edu/~sedwards/classes/2016/4840-spring/designs/Chip8.pdf) - Academic reference
- [CHIP-8 Fontset](https://en.wikipedia.org/wiki/CHIP-8#Font_set) - Standard character sprites
- [CHIP-8 ROMs](https://github.com/kripod/chip8-roms) - Extensive collection of ROM files

All the roms included in this project are taken from the above collection of CHIP-8 ROMs, which is a comprehensive archive of games, demos, and programs for the CHIP-8 platform

Special thanks to Tobias V. I. Langhoff for his detailed guide on writing a CHIP-8 emulator, which served as a valuable resource during development.

Special thanks to kripod for maintaining such an extensive collection of CHIP-8 ROMs and providing detailed information about each one.

### Additional Resources

- [CHIP-8 Opcode Table](https://en.wikipedia.org/wiki/CHIP-8#Opcode_table) - Quick reference
- [Raylib Documentation](https://www.raylib.com/) - Graphics library reference

## License

This project is for educational purposes. ROM files may have their own licenses.

## Contributing

Feel free to submit issues or pull requests for:

- Bug fixes
- Performance improvements
- Additional features (Super CHIP-8, XO-CHIP support)
- Documentation improvements
