# CHIP-8 Emulator in C++

A faithful CHIP-8 interpreter written in C++, supporting all 35 opcodes and providing a complete execution environment with graphics, input, and timing.

## Features

- **Full Opcode Support**  
  Parsing, decoding, and execution of all 35 CHIP-8 instructions.

- **Memory & Registers**  
  4 KB memory space, 16 general-purpose 8-bit registers, 16-level stack, and program counter management.

- **SDL2 Graphics & Input**  
  64×32 monochrome display rendering, keyboard input handling, and frame synchronization.

- **Accurate Timing**  
  CPU cycle rate at 500 Hz, plus precise delay and sound timers.

- **ROM Loader & Debugging**  
  Load arbitrary CHIP-8 ROMs and inspect memory/register state for debugging.
