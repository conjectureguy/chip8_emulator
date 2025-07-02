#include <iostream>
#include <chrono>
#include "../include/Chip8.h"
#include "../include/Platform.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        exit(1);
    }

    const char *filename = argv[1];

    Chip8 chip8;
    chip8.LoadRom(filename);
    int videoScale = 10;
    float cycleDelay = 0.0167;

    int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

	Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit) {
		quit = platform.processInput(chip8.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay) {
			lastCycleTime = currentTime;
			chip8.Cycle();
			platform.update(chip8.video, videoPitch);
		}
	}

	return 0;
}