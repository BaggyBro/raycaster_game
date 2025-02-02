
#include "../include/game.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <math.h>

#define PI 3.1415926535
#define DR (PI/360)

float dist(float ax, float ay, float bx, float by, float ang){
  return (sqrt((bx-ax)*(bx-ax) + (by-ay)*(by-ay)));
}


void castRay(float x, float y, float dx, float dy, float a, int MAP_SIZE, int *MAP) {
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo,distT;
    float disV = 1000000, vx = x, vy = y;
    float disH = 1000000, hx = x, hy = y;

    ra = a - DR*90;

    if(ra < 0) {ra += 2*PI;}
    if(ra > 2*PI) {ra -= 2*PI;}
    
  for(r = 0; r <= 180; r++){
    // HORIZONTAL RAYCASTING
    dof = 0;
    float aTan = (fabs(tan(ra)) > 0.0001) ? -1 / tan(ra) : 0;  // Avoid division by zero

    if (ra > M_PI) {
        ry = (((int)y / 64) * 64) - 0.0001;
        rx = (y - ry) * aTan + x;
        yo = -64;
        xo = -yo * aTan;
    } else if (ra < M_PI) {
        ry = (((int)y / 64) * 64) + 64;
        rx = (y - ry) * aTan + x;
        yo = 64;
        xo = -yo * aTan;
    } else {
        rx = x;
        ry = y;
        dof = 8;
    }

    while (dof < 8) {
        mx = (int)(rx) / 64;
        my = (int)(ry) / 64;

        if (mx >= 0 && mx < MAP_SIZE && my >= 0 && my < MAP_SIZE) {
            mp = my * MAP_SIZE + mx;
            if (MAP[mp] == 1) {
                hx = rx;
                hy = ry;
                disH = dist(x, y, hx, hy, a);
                break;
            }
        } else {
            break;
        }

        rx += xo;
        ry += yo;
        dof++;
    }

    // VERTICAL RAYCASTING
    dof = 0;
    float nTan = (fabs(tan(ra)) > 0.0001) ? -tan(ra) : 0;  // Avoid division by zero

    if (ra > M_PI / 2 && ra < 3 * M_PI / 2) {
        rx = (((int)x / 64) * 64) - 0.0001;
        ry = (x - rx) * nTan + y;
        xo = -64;
        yo = -xo * nTan;
    } else if (ra < M_PI / 2 || ra > 3 * M_PI / 2) {
        rx = (((int)x / 64) * 64) + 64;
        ry = (x - rx) * nTan + y;
        xo = 64;
        yo = -xo * nTan;
    } else {
        rx = x;
        ry = y;
        dof = 8;
    }

    while (dof < 8) {
        mx = (int)(rx) / 64;
        my = (int)(ry) / 64;

        if (mx >= 0 && mx < MAP_SIZE && my >= 0 && my < MAP_SIZE) {
            mp = my * MAP_SIZE + mx;
            if (MAP[mp] == 1) {
                vx = rx;
                vy = ry;
                disV = dist(x, y, vx, vy,a);
                break;
            }
        } else {
            break;
        }

        rx += xo;
        ry += yo;
        dof++;
    }

    if (disV < disH) {
        rx = vx;
        ry = vy;
        distT = disV;
        glColor3f(0.9,0,0);
    } else {
        rx = hx;
        ry = hy;
        distT = disH;
        glColor3f(0.7,0,0);
    }

    // Draw the ray
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2i(x, y);
    glVertex2i(rx, ry);
    glEnd();

    //3D starts here T-T
 /*   float FOV = 120 * (PI / 180);
    float projPlaneDist = (1980 / 2) / tan(FOV / 2);  // Projection plane distance
    float lineH = (64 * projPlaneDist) / distT;  // Scale with the projection plane
    if(lineH > 1080) lineH = 1080;  // Clamp to screen height
    float lineO = 1080/2 - lineH/2;
*/  
    float ca = ra - a; if(ca<0){ca+=2*PI;} if(ca>2*PI){ca-=2*PI;} distT = distT*cos(ca); //fisheye fix
    float lineH = (MAP_SIZE*2560)/ distT; if(lineH > 2560) lineH = 2560;   //line height
    float lineO = 320 - lineH/2;                                           //line offset
   // glLineWidth(8); glBegin(GL_LINES); glVertex2i(r*8+530,lineO); glVertex2i(r*8+530+1, lineH+lineO);glEnd();
   glBegin(GL_QUADS);
    glVertex2i(r * 8 + 530, lineO);
    glVertex2i(r * 8 + 538, lineO);
    glVertex2i(r * 8 + 538, lineH + lineO);
    glVertex2i(r * 8 + 530, lineH + lineO);
    glEnd();
    
    ra+=DR;    
    if(ra < 0) {ra += 2*PI;}
    if(ra > 2*PI) {ra -= 2*PI;}
    

    }
}


void drawPlayer(float x, float y, float dx, float dy){
  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();

  glLineWidth(3);
  glBegin(GL_LINES);
  glVertex2i(x,y);
  glVertex2i(x+dx*5,y+dy*5);
  glEnd();

}

void handleInput(float *x, float *y, float* dx, float* dy, float *da, float speed) {
    const Uint8* key = SDL_GetKeyboardState(NULL);

    if (key[SDL_SCANCODE_A]) {
        (*da) -= 0.1;
        if((*da) < 0){
          (*da) += 2*PI;
        }
        (*dx) = cos((*da)) * 5;
        (*dy) = sin((*da)) * 5;
    }
    if (key[SDL_SCANCODE_D]) {
        (*da) += 0.1;
        if((*da) >  2*PI){
          (*da) -= 2*PI;
        }
        (*dx) = cos((*da)) * 5;
        (*dy) = sin((*da)) * 5;
    }
    if (key[SDL_SCANCODE_W]) {
        (*x) += (*dx);
        (*y) += (*dy);
    }
    if (key[SDL_SCANCODE_S]) {
        (*x) -= (*dx);
        (*y) -= (*dy);       
    }
}

void drawMap(int MAP_SIZE, int *MAP){
  int SIZE = 64;
  int xo, yo;

  for(int y = 0; y < MAP_SIZE; y++){
    for(int x = 0 ; x < MAP_SIZE; x++){
      if(MAP[y*8 + x] == 0) glColor3f(0,0,0); else glColor3f(1,1,1);
      xo = x * SIZE; yo = y * SIZE;
      glBegin(GL_QUADS);
      glVertex2i(xo + 1, yo + 1) ;
      glVertex2i(xo + 1, yo + SIZE - 1);
      glVertex2i(xo + SIZE - 1, yo + SIZE - 1);
      glVertex2i(xo + SIZE - 1, yo + 1);
      glEnd();

    }
  }
}

