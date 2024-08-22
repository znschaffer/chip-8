# Chip-8 Emulator

Chip-8 emulator written in C using SDL2 for graphics rendering. The emulator interprets Chip-8 ROMs, emulating the original Chip-8 system, including the CPU, memory, stack, and screen display.

## Features

- **Chip-8 Emulation**: Emulates the Chip-8 system, including memory, registers, stack, timers, and input handling.
- **Graphics Rendering**: Uses SDL2 to render the Chip-8's 64x32 pixel display at a scalable resolution.
- **ROM Loading**: Loads Chip-8 ROMs into memory and starts execution.

## Dependencies

- SDL2 (Simple DirectMedia Layer 2)
  
You can install SDL2 on Ubuntu using the following command:

```bash
sudo apt-get install libsdl2-dev
```

## Build Instructions
To build the Chip-8 emulator, you will need to have a C compiler (e.g., GCC) and SDL2 installed. Use the following command to compile the project:

```bash
gcc -o chip8 main.c core.c -lSDL2
```

## Usage

```bash
./chip8 path/to/rom
```

## Project Structure
- **main.c**: The entry point of the program. It handles SDL initialization, the game loop, and rendering the screen.
- **core.c**: Implements the Chip-8 CPU and memory. It handles the fetch-decode-execute cycle, stack operations, and emulation of Chip-8 instructions.
- **core.h**: Header file containing constants, structure definitions, and function declarations for the emulator's core logic.

## Chip-8 Overview
Chip-8 is an interpreted programming language that was first used on some early home computers in the mid-1970s. It was designed to allow easy creation of video games. Chip-8 programs are loaded into memory and executed by the virtual machine.

The system features:
- Memory: 4KB of RAM
- Display: 64x32 monochrome display
- Registers: 16 general-purpose 8-bit registers (V0-VF)
- Stack: Used for storing return addresses for subroutines
- Timers: Delay and sound timers
- Input: A hexadecimal keypad (0-9, A-F)
