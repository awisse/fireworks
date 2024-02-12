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

void InitStar();
void StepFireworks();

// Functions
void InitGame() {

  animating = false;
  dissolving = false;
  modified = true;
  InitStar();
  Platform::Clear();
}

bool StepGame() {

  HandleInput();

  if (animating) {
    StepFireworks();
  }

  if (modified) {
    modified = false;
    return true;
  }
  return false;
}

void InitRay(Ray* ray) {

  ray->dx = ABSDIFF(ray->x0, ray->x);
  ray->sx = SIGNXY(ray->x, ray->x0);
  ray->dy = ABSDIFF(ray->y0, ray->y);
  ray->sy = SIGNXY(ray->y, ray->y0);
  ray->err = (ray->dx > ray->dy ? ray->dx : -ray->dy) >> 1;
}

void rotate(Ray* tgt, const Ray* src) {
  // tgt is src rotated clockwise by Pi/6
  // Method: Multiplication by rotation matrix
  tgt->x0 = (src->x0 * COS30 - (src->y0 << 6)) >> 7;
  tgt->y0 = ((src->x0 << 6) + src->y0 * COS30) >> 7;
}

void InitStar() {
  // Prepare the Star structure to be reused for each explosion
  uint16_t i;

  // Zero out the whole structure
  for (i = 0; i < (uint16_t)sizeof(star); i++) {
    *((uint8_t*)star + i) = 0;
  }

  // Compute the generic endpoints of NUM_RAYS stars
  for (i = 0; i < NUM_RAYS / 3; i++) { // Four quadrants simultaneously
    // 0, Pi/2, Pi, 3*Pi/2
    star[i * 3].x0 = ((i & 1) - (i & 2)) * (i & 1) * RAY_LENGTH;
    star[i * 3].y0 = ((~i) & 1) * ((i & 2) - 1) * RAY_LENGTH;
    InitRay(&star[i * 3]);
    // Pi/6, 2*Pi/3, 7*Pi/6, 5*Pi/3
    rotate(&star[i * 3 + 1], &star[i * 3]);
    InitRay(&star[i * 3 + 1]);
    // Pi/3, 5*Pi/6, 4*Pi/3, 11*Pi/6
    rotate(&star[i * 3 + 2], &star[i * 3 + 1]);
    InitRay(&star[i * 3 + 2]);
#ifdef _DEBUG
    for (uint8_t j=0; j<3; j++) {
      Platform::DebugPrint((uint16_t)((i * 3 + j) * 30));
      Platform::DebugPrint((int16_t)star[3*i+j].x0);
      Platform::DebugPrint((int16_t)star[3*i+j].y0, true);
    }
#endif
  }
}

void StartFireworks() {
  uint8_t i;
  int8_t x0 = Platform::Random(32, 96);
  int8_t y0 = Platform::Random(16, 48);
  // Translate the ray endpoints
  for (i = 0; i < NUM_RAYS; i++) {
    star[i].x = x0;
    star[i].y = y0;
    star[i].x1 = x0 + star[i].x0;
    star[i].y1 = y0 + star[i].y0;
  }
  Platform::Clear();
  animating = true;
}

void ShowStar() {
  StartFireworks();
  DrawStar(star);
  modified=true;
}

void StartLine() {
  // Draw a line: Test the line algorithm
  DrawLine(16, 16, 64, 32);
  modified = true;
  dissolving = true;
}

void StepFireworks() {
  // Move the fireworks one step further
  // Not used for now.
  int8_t e2;
  uint8_t i, finished = 0;

  for (i=0; i<NUM_RAYS; i++) {

    Platform::PutPixel(star[i].x, star[i].y);

    if (star[i].x==star[i].x1 && star[i].y==star[i].y1) {
      finished++;
      continue;
    }

    e2 = star[i].err;
    if (e2 >-star[i].dx) {
      star[i].err -= star[i].dy;
      star[i].x += star[i].sx;
    }
    if (e2 < star[i].dy) {
      star[i].err += star[i].dx;
      star[i].y += star[i].sy;
    }
  }
  animating = finished < NUM_RAYS;
  modified = true;
}

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
