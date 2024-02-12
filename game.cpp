/*
Helper functions to unclutter main .ino file
 */
#include "game.h"
#include "draw.h"
#include "controller.h"
#include "globals.h"
#include "platform.h"

// Global variables
bool modified; // True if screen needs to be redrawn
bool animating;
uint8_t linepos;
bool dissolving;
Ray star[NUM_RAYS]; // A star from NUM_RAYS rays
Ray ray;

void StepLine();
void StepFireworks();

// Functions
void InitGame() {

  animating = false;
  dissolving = false;
  modified = false;
  Platform::Clear();
}

void StepGame() {

  HandleInput();

  if (animating) {
    StepFireworks();
  }

  if (modified) {
    DrawStar(star);
    modified = false;
  }

}

void InitRay(Ray* ray) {
  
  ray->dx = ABSDIFF(ray->x1, ray->x0);
  ray->sx = SIGNXY(ray->x0, ray->x1);
  ray->dy = ABSDIFF(ray->y1, ray->y0);
  ray->sy = SIGNXY(ray->y0, ray->y1);
  ray->err = (ray->dx > ray->dy ? ray->dx : -ray->dy) >> 1;
}

void rotate(Ray* tgt, const Ray* src) {
  // tgt is src rotated clockwise by Pi/6
  // Method: Multiplication by rotation matrix
  tgt->x1 = (src->x1 * COS30 - (src->y1 << 9)) >> 10;
  tgt->y1 = ((src->x1 << 9) + src->y1 * COS30) >> 10;
}

void StartFireworks() {
  // Start a new firework
  int8_t i, j; // Loop variable
  constexpr int8_t x0 = 64;
  constexpr int8_t y0 = 32;
                  
  // Develop NUM_RAYS Stars
  for (i = 0; i < NUM_RAYS / 3; i++) { // Four quadrants simultaneously
    // 0, Pi/2, Pi, 3*Pi/2
    star[i * 3].x1 = ((i & 1) - (i & 2)) * (i & 1) * RAY_LENGTH;
    star[i * 3].y1 = ((~i) & 1) * ((i & 2) - 1) * RAY_LENGTH;
    InitRay(&star[i * 3]);
    // Pi/6, 2*Pi/3, 7*Pi/6, 5*Pi/3
    rotate(&star[i * 3 + 1], &star[i * 3]);
    InitRay(&star[i * 3 + 1]);
    // Pi/3, 5*Pi/6, 4*Pi/3, 11*Pi/6 
    rotate(&star[i * 3 + 2], &star[i * 3 + 1]);
    InitRay(&star[i * 3 + 2]);

    // Add Origin
    for (j = 0; j < 3; j++) {
      star[i * 3 + j].x0 = x0;
      star[i * 3 + j].y0 = y0;
      star[i * 3 + j].x1 += x0;
      star[i * 3 + j].y1 += y0;
    }
  }
  Platform::Clear();
  modified = true;
}

void StartLine() {
  // Draw a line: Test the line algorithm
  DrawLine(16, 16, 64, 32);
  modified = true;
  dissolving = true;
}



void StepLine() {
  // Move the fireworks one step further
  // Not used for now.
  static int8_t e2;

  Platform::PutPixel(ray.x0, ray.y0, COLOUR_WHITE);  

  if (ray.x0==ray.x1 && ray.y0==ray.y1) 
    animating = false;

  e2 = ray.err;
  if (e2 >-ray.dx) { 
    ray.err -= ray.dy; ray.x0 += ray.sx; 
  }

  if (e2 < ray.dy) { 
    ray.err += ray.dx; ray.y0 += ray.sy; 
  }
}

void StepFireworks() {
}

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
