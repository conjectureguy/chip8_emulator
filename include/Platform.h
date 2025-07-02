#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL2/SDL.h>
#include "glad/glad.h"

class Platform {
    friend class Imgui;

public:
    Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~Platform();
    void update(void const* buffer, int pitch);
    bool processInput(uint8_t* keys);

private:
    SDL_Window* window{};
    SDL_GLContext gl_context{};
    GLuint framebuffer_texture{};
   	SDL_Renderer* renderer{};
	SDL_Texture* texture{};
};



#endif //PLATFORM_H
