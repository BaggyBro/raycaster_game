
#include "../include/game.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>

#define WIDTH 1024
#define HEIGHT 512
#define SPEED 2.0f
#define MAP_SIZE 8
#define PI 3.1415926535

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("Failed to create OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Set up OpenGL
    SDL_GL_SetSwapInterval(1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float x = WIDTH / 2;
    float y = HEIGHT / 2;
    float da = 0;
    float dx = cos(da)*5;
    float dy = sin(da)*5;

    int MAP[] = {
      1,1,1,1,1,1,1,1,
      1,0,0,0,0,0,0,1,
      1,0,0,0,0,0,0,1,
      1,0,1,1,1,0,0,1,
      1,0,0,1,1,0,0,1,
      1,0,0,0,0,1,0,1,
      1,0,0,0,0,0,1,1,
      1,1,1,1,1,1,1,1
    };

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        handleInput(&x, &y, &dx, &dy, &da,  SPEED);

        glClear(GL_COLOR_BUFFER_BIT);
        drawMap(MAP_SIZE, MAP);
        drawPlayer(x, y, dx, dy);
        castRay(x, y, dx, dy, da, MAP_SIZE, MAP);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

