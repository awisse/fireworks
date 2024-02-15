/*
Helper functions to unclutter main .ino file
 */
#include "game.h"
#include "draw.h"
#include "controller.h"
#include "globals.h"
#include "utils.h"
#include "platform.h"

// Global variables
bool modified; // True if screen needs to be redrawn
bool animating;
bool dissolving;
Star star[MAX_FIREWORKS]; // A star[0] from NUM_RAYS rays
uint8_t erase_radius = 0;

void InitStar();
void StepFireworks();
void StepDissolve();

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

  if (dissolving) {
    StepDissolve();
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
  for (i = 0; i < (uint16_t)sizeof(star[0].rays); i++) {
    *((uint8_t*)star[0].rays + i) = 0;
  }

  // Compute the generic endpoints of NUM_RAYS stars
  for (i = 0; i < NUM_RAYS / 3; i++) { // Four quadrants simultaneously
    // 0, Pi/2, Pi, 3*Pi/2
    star[0].rays[i * 3].x0 = ((i & 1) - (i & 2)) * (i & 1) * RAY_LENGTH;
    star[0].rays[i * 3].y0 = ((~i) & 1) * ((i & 2) - 1) * RAY_LENGTH;
    InitRay(&star[0].rays[i * 3]);
    // Pi/6, 2*Pi/3, 7*Pi/6, 5*Pi/3
    rotate(&star[0].rays[i * 3 + 1], &star[0].rays[i * 3]);
    InitRay(&star[0].rays[i * 3 + 1]);
    // Pi/3, 5*Pi/6, 4*Pi/3, 11*Pi/6
    rotate(&star[0].rays[i * 3 + 2], &star[0].rays[i * 3 + 1]);
    InitRay(&star[0].rays[i * 3 + 2]);
  }
  star[0].R0 = -star[0].rays[0].y0;
}

void StartFireworks() {
  uint8_t i;
  int8_t x0 = Platform::Random(32, 96);
  int8_t y0 = Platform::Random(16, 48);
  // Translate the ray endpoints
  star[0].X = x0;
  star[0].Y = y0;
  star[0].R = 0;
  for (i = 0; i < NUM_RAYS; i++) {
    star[0].rays[i].x = x0;
    star[0].rays[i].y = y0;
    star[0].rays[i].x1 = x0 + star[0].rays[i].x0;
    star[0].rays[i].y1 = y0 + star[0].rays[i].y0;
  }
  Platform::Clear();
  animating = true;
  dissolving = false;
}

void ToggleAnimation() {
  animating = !animating;
}

void StepFireworks() {
  // Move the fireworks one step further
  // Not used for now.
  int8_t e2;
  uint8_t i, finished = 0;

  for (i=0; i<NUM_RAYS; i++) {

    Platform::PutPixel(star[0].rays[i].x, star[0].rays[i].y);

    if (star[0].rays[i].x==star[0].rays[i].x1 && star[0].rays[i].y==star[0].rays[i].y1) {
      finished++;
      continue;
    }

    e2 = star[0].rays[i].err;
    if (e2 >-star[0].rays[i].dx) {
      star[0].rays[i].err -= star[0].rays[i].dy;
      star[0].rays[i].x += star[0].rays[i].sx;
    }
    if (e2 < star[0].rays[i].dy) {
      star[0].rays[i].err += star[0].rays[i].dx;
      star[0].rays[i].y += star[0].rays[i].sy;
    }
  }
  star[0].R = star[0].Y - star[0].rays[0].y;
  if ((star[0].R > star[0].R0 / 2) && !dissolving) {
    dissolving = true;
    erase_radius = star[0].R0 / 3;
  }
  animating = finished < NUM_RAYS;
  modified = true;
}

void StepDissolve() {
  // One step of dissolving the star[0]
  // Start dissolving when star[0] is half done

#ifdef _DEBUG
  Platform::DebugPrint((uint8_t*)"erase_radius=");
  Platform::DebugPrint((uint16_t)erase_radius);
  Platform::DebugPrint((uint8_t*)"star[0].R=");
  Platform::DebugPrint((uint16_t)star[0].R, true);
#endif
  if (animating) {
    DrawScatterDisk(star[0].X, star[0].Y, star[0].R, 2, COLOUR_BLACK);
  } else {
    Platform::DrawFilledCircle(star[0].X, star[0].Y, erase_radius, 
        COLOUR_BLACK);
    erase_radius += 4;
    if (erase_radius > star[0].R0) dissolving = false;
  }
  modified = true;
}

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
