#include "../include/Chip8.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <iosfwd>
#include <iostream>

const unsigned int FONTSET_SIZE = 80;

uint8_t fontset[FONTSET_SIZE] =
{
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


Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
    pc = START_ADDRESS; // instruction pointer

	// characters stored from 0x050 to 0x0A0 in memory
	// Loading fontset to memory
	for (int i = 0; i < FONTSET_SIZE; i++) {
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	// Initialize RNG from 0 to 255
	randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void Chip8::LoadRom(const char *filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        auto size = file.tellg();
        file.seekg(0, std::ios::beg);
        char *buffer = new char[size];
        file.read(buffer, size);
        file.close();

        // ROM starts at 0x200
        for (int i = 0; i < size; i++) {
            memory[START_ADDRESS + i] = buffer[i];
        }
        delete[] buffer;
    }
}

void Chip8::Cycle() {
	opcode = (memory[pc] << 8) | memory[pc + 1];
	pc += 2;

	int opcode_msb_nibble = (opcode & 0xFFFF) >> 12;
	// std::cout << opcode << std::endl;

	switch (opcode_msb_nibble) {
		case 0:
			switch (opcode) {
				case 0x00E0:
					OP_00E0();
					break;
				case 0x00EE:
					OP_00EE();
					break;
			default:
					std:: cerr << opcode_msb_nibble << std::endl;
					std::cerr << "Unknown opcode: " << opcode << std::endl;
					break;
			}
			break;
		case 1:
			OP_1nnn();
			break;
		case 2:
			OP_2nnn();
			break;
		case 3:
			OP_3xkk();
			break;
		case 4:
			OP_4xkk();
			break;
		case 5:
			OP_5xy0();
			break;
		case 6:
			OP_6xkk();
			break;
		case 7:
			OP_7xkk();
			break;
		case 8: {
			int val = opcode & 0x000F;
			switch (val) {
				case 0:
					OP_8xy0();
					break;
				case 1:
					OP_8xy1();
					break;
				case 2:
					OP_8xy2();
					break;
				case 3:
					OP_8xy3();
					break;
				case 4:
					OP_8xy4();
					break;
				case 5:
					OP_8xy5();
					break;
				case 6:
					OP_8xy6();
					break;
				case 7:
					OP_8xy7();
					break;
				case 0xE:
					OP_8xyE();
					break;
				default:
					std:: cerr << opcode_msb_nibble << std::endl;
					std::cerr << "Unknown opcode: " << opcode << std::endl;
					break;
			}
			break;
		}
		case 9:
			OP_9xy0();
			break;
		case 10:
			OP_Annn();
			break;
		case 11:
			OP_Bnnn();
			break;
		case 12:
			OP_Cxkk();
			break;
		case 13:
			OP_Dxyn();
			break;
		case 14: {
			int val = opcode & 0x00FF;
			switch (val) {
				case 0x9E:
					OP_Ex9e();
					break;
				case 0xA1:
					OP_ExA1();
					break;
				default:
					std:: cerr << opcode_msb_nibble << std::endl;
					std::cerr << "Unknown opcode: " << opcode << std::endl;
					break;
			}
			break;
		}
		case 15: {
			int val = opcode & 0x00FF;
			switch (val) {
				case 0x07:
					OP_Fx07();
					break;
				case 0x0A:
					OP_Fx0A();
					break;
				case 0x15:
					OP_Fx15();
					break;
				case 0x18:
					OP_Fx18();
					break;
				case 0x1E:
					OP_Fx1E();
					break;
				case 0x29:
					OP_Fx29();
					break;
				case 0x33:
					OP_Fx33();
					break;
				case 0x55:
					OP_Fx55();
					break;
				case 0x65:
					OP_Fx65();
					break;
				default:
					std:: cerr << opcode_msb_nibble << std::endl;
					std::cerr << "Unknown opcode: " << opcode << std::endl;
					break;
			}
			break;
		}
		default:
			std::cerr << "Unknown opcode nibble: " << opcode_msb_nibble << std::endl;
			break;
	}

	if (delayTimer) {
		delayTimer--;
	}

	if (soundTimer) {
		soundTimer--;
	}
}


// Instruction functions

void Chip8::OP_NULL() {
}

void Chip8::OP_00E0() {
	// Clear the display
	memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE() {
	// Return from a subroutine
	sp--;
	pc = stack[sp];
}

void Chip8::OP_1nnn() {
	// Jump to location nnn
	uint16_t address = opcode & 0x0FFF;
	pc = address;
}

void Chip8::OP_2nnn() {
	// Call subroutine at nnn
	uint16_t address = opcode & 0x0FFF;
	stack[sp] = pc;
	sp++;
	pc = address;
}

void Chip8::OP_3xkk() {
	// Skip next instruction if Vx = kk
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t bytes = opcode & 0x00FF;

	if (registers[Vx] == bytes) {
		pc += 2;
	}
}

void Chip8::OP_4xkk() {
	// Skip next instruction if Vx != kk
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t bytes = opcode & 0x00FF;

	if (registers[Vx] != bytes) {
		pc += 2;
	}
}

void Chip8::OP_5xy0() {
	// Skip next instruction if Vx = Vy
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vx] == registers[Vy]) {
		pc += 2;
	}
}

void Chip8::OP_6xkk() {
	// Set Vx = kk
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t bytes = opcode & 0x00FF;

	registers[Vx] = bytes;
}

void Chip8::OP_7xkk() {
	// Set Vx = Vx + kk
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t bytes = opcode & 0x00FF;

	registers[Vx] += bytes;
}

void Chip8::OP_8xy0() {
	// Set Vx = Vy
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] = registers[Vy];
}

void Chip8::OP_8xy1() {
	// Set Vx = Vx OR Vy
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] |= registers[Vy];
}

void Chip8::OP_8xy2() {
	// Set Vx = Vx AND Vy
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] &= registers[Vy];
}

void Chip8::OP_8xy3() {
	// Set Vx = Vx XOR Vy
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] ^= registers[Vy];
}

void Chip8::OP_8xy4() {
	// Set Vx = Vx + Vy, set VF = carry
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	uint16_t sum = registers[Vx] + registers[Vy];
	if (sum > 255) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}

	registers[Vx] = sum & 0xFF;
}

void Chip8::OP_8xy5() {
	// Set Vx = Vx - Vy, set VF = NOT borrow
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vx] > registers[Vy]) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}

	registers[Vx] -= registers[Vy];
}

void Chip8::OP_8xy6() {
	// Set Vx = Vx SHR 1
	// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	if (registers[Vx] & 1) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}
	registers[Vx] >>= 1;
}

void Chip8::OP_8xy7() {
	// Set Vx = Vy - Vx, set VF = NOT borrow
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vy] > registers[Vx]) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}

	registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip8::OP_8xyE() {
	// Set Vx = Vx SHL 1
	// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	registers[0xF] = (registers[Vx] & 0x80) >> 7;
	registers[Vx] <<= 1;
}

void Chip8::OP_9xy0() {
	// Skip next instruction if Vx != Vy
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vx] != registers[Vy]) {
		pc += 2;
	}
}

void Chip8::OP_Annn() {
	// Set I = nnn
	uint16_t address = opcode & 0x0FFF;

	index = address;
}

void Chip8::OP_Bnnn() {
	 // Jump to location nnn + V0
	uint16_t address = opcode & 0x0FFF;

	pc = registers[0] + address;
}

void Chip8::OP_Cxkk() {
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t bytes = opcode & 0x00FF;

	registers[Vx] = randByte(randGen) & bytes;
}

void Chip8::OP_Dxyn() {
	// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
	static int drawCount = 0;
	// std::cout << "Drawing " << drawCount++ << std::endl;
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	uint8_t height = opcode & 0x000F;

	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; row++) {
		uint8_t spriteRow = memory[index + row];
		for (unsigned int col = 0; col < 8; col++) {
			uint8_t spritePixel = spriteRow & (0x80 >> col);
			uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			if (spritePixel) {
				// std::cout << "ON\n";
				if (*screenPixel == 0xFFFFFFFF)
					registers[0xF] = 1;

				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::OP_Ex9e() {
	// Skip next instruction if key with the value of Vx is pressed
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[Vx];

	if (keypad[key]) {
		pc += 2;
	}
}

void Chip8::OP_ExA1() {
	// Skip next instruction if key with the value of Vx is not pressed
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[Vx];

	if (!keypad[key]) {
		pc += 2;
	}
}

void Chip8::OP_Fx07() {
	// Set Vx = delay timer value
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	registers[Vx] = delayTimer;
}

void Chip8::OP_Fx0A() {
	// Wait for a key press, store the value of the key in Vx
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	bool found = false;
	for (unsigned int i = 0; i < 16; i++) {
		if (keypad[i]) {
			found = true;
			registers[Vx] = i;
			// std::cout << "Keypress: " << i << std::endl;
			break;
		}
	}
	if (!found) {
		pc -= 2;
	}
}

void Chip8::OP_Fx15() {
	// Set delay timer = Vx
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	delayTimer = registers[Vx];
}

void Chip8::OP_Fx18() {
	// Set sound timer = Vx
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	soundTimer = registers[Vx];
}

void Chip8::OP_Fx1E() {
	// Set I = I + Vx
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	index += registers[Vx];
}

void Chip8::OP_Fx29() {
	// Set I = location of sprite for digit Vx
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t digit = registers[Vx];

	index = FONTSET_START_ADDRESS + 5 * digit;
}

void Chip8::OP_Fx33() {
	// Store BCD representation of Vx in memory locations I, I+1, and I+2
	// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I,
	// the tens digit at location I+1, and the ones digit at location I+2.
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t value = registers[Vx];

	// Ones digit
	memory[index + 2] = value % 10;
	value /= 10;

	// Tens digit
	memory[index + 1] = value % 10;
	value /= 10;

	// Hundreds digit
	memory[index] = value % 10;
}

void Chip8::OP_Fx55() {
	// Store registers V0 through Vx in memory starting at location I
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	for (unsigned int i = 0; i <= Vx; i++) {
		memory[index + i] = registers[i];
	}
}

void Chip8::OP_Fx65() {
	// Read registers V0 through Vx from memory starting at location I
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	for (unsigned int i = 0; i <= Vx; i++) {
		registers[i] = memory[index + i];
	}
}