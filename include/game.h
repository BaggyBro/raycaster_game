#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <stdio.h>

void handleInput(float* x, float* y,float* dx, float* dy, float * da, float speed);
void drawMap(int MAP_SIZE, int MAP[]);
void drawPlayer(float x, float y, float dx, float dy);
void castRay(float x, float y, float dx, float dy, float a, int MAP_SIZE, int MAP[]);
#endif
