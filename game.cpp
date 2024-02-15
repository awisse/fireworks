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
bool running; // Run continuous fireworks
Firework fireworks[MAX_FIREWORKS];
uint8_t erase_radius;
uint8_t oldest_fw;

void InitFireworks();
void Init1Firework(Firework* firework);
void EraseFirework(Firework* firework);
void StartFirework();
void Start1Firework();
void StepFireworks();
void Step1Firework(Firework* firework);
void StepDissolve(Firework* firework);

// Functions
void InitGame() {

  animating = false;
  modified = true;
  running = false;
  InitFireworks();
  Platform::Clear();
}

bool StepGame() {

  HandleInput();

  if (animating) {
    StepFireworks();
  }

  if (running) {
    StartFirework();
  }

  if (modified) {
    modified = false;
    return true;
  }
  return false;
}

void InitFireworks() {
  uint8_t i;

  for (i=0; i<MAX_FIREWORKS; i++) {
    Init1Firework(&fireworks[i]);
  }
  oldest_fw = 0;
}

void EraseFirework(Firework* firework) {

  uint16_t i;

  // Zero out the whole structure
  for (i = 0; i < (uint16_t)sizeof(*firework); i++) {
    *((uint8_t*)firework + i) = 0;
  }
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

void Init1Firework(Firework* fw) {
  // Prepare the Firework structure to be reused for each explosion
  uint16_t i; // Must be a 16 bit variable

  EraseFirework(fw);

  // Compute the generic endpoints of NUM_RAYS stars
  for (i = 0; i < NUM_RAYS / 3; i++) { // Four quadrants simultaneously

    // 0, Pi/2, Pi, 3*Pi/2
    fw->rays[i * 3].x0 = ((i & 1) - (i & 2)) * (i & 1) * RAY_LENGTH;
    fw->rays[i * 3].y0 = ((~i) & 1) * ((i & 2) - 1) * RAY_LENGTH;
    InitRay(&fw->rays[i * 3]);

    // Pi/6, 2*Pi/3, 7*Pi/6, 5*Pi/3
    rotate(&fw->rays[i * 3 + 1], &fw->rays[i * 3]);
    InitRay(&fw->rays[i * 3 + 1]);

    // Pi/3, 5*Pi/6, 4*Pi/3, 11*Pi/6
    rotate(&fw->rays[i * 3 + 2], &fw->rays[i * 3 + 1]);
    InitRay(&fw->rays[i * 3 + 2]);
  }
}

void ToggleFireworks() {
  running = !running;
}

void StartFirework() {
  static uint32_t last_time = 0;
  uint32_t current_time = Platform::Millis();

  if ((current_time > last_time + FW_DELAY) && running) {
    last_time = current_time;
    Start1Firework();
  }
}

void Start1Firework() {
  uint8_t fwnum, i;
  int8_t x0;
  int8_t y0;
  Firework* fw;

  // Find empty spot in fireworks array
  for (fwnum=0; fwnum<MAX_FIREWORKS; fwnum++) {
    fw = &fireworks[(oldest_fw + fwnum) % MAX_FIREWORKS];
    if (!(fw->growing || fw->dissolving)) break;
  }
  if (fwnum == MAX_FIREWORKS) { // No room for another one
    return;
  }

  fw = fireworks + fwnum;

  x0 = Platform::Random(32, 96);
  y0 = Platform::Random(16, 48);

  // Translate the ray endpoints
  for (i = 0; i < NUM_RAYS; i++) {
    fw->rays[i].x = x0;
    fw->rays[i].y = y0;
    fw->rays[i].x1 = x0 + fw->rays[i].x0;
    fw->rays[i].y1 = y0 + fw->rays[i].y0;
  }
  fw->X = x0;
  fw->Y = y0;
  fw->R = 0;
  fw->growing = true;
  fw->dissolving = false;

  animating = true;
}

void ToggleAnimation() {
  animating = !animating;
  running = !running || animating;
}

void StepFireworks() {
  // Move the fireworks one step further
  // Not used for now.
  uint8_t i;
  bool alive = false;
  Firework* fw;

  // IDEA: Draw fireworks in order, such that dissolving fireworks are
  // overwritten by drawing fireworks.

  for (i=0; i<MAX_FIREWORKS; i++) {
    fw = &fireworks[(oldest_fw + i) % MAX_FIREWORKS];
    if (fw->growing) {
      alive = true;
      Step1Firework(fw);
    }
    if (fw->dissolving) {
      alive = true;
      StepDissolve(fw);
    }
  }
  // Stop drawing updates if no firework alive anymore
  modified = animating = alive;
}

void Step1Firework(Firework* fw) {

  int8_t e2;
  uint8_t i, finished = 0;

  for (i=0; i<NUM_RAYS; i++) {

    Platform::PutPixel(fw->rays[i].x, fw->rays[i].y);

    if (fw->rays[i].x==fw->rays[i].x1 &&
        fw->rays[i].y==fw->rays[i].y1) {
      finished++;
      continue;
    }

    e2 = fw->rays[i].err;
    if (e2 >-fw->rays[i].dx) {
      fw->rays[i].err -= fw->rays[i].dy;
      fw->rays[i].x += fw->rays[i].sx;
    }
    if (e2 < fw->rays[i].dy) {
      fw->rays[i].err += fw->rays[i].dx;
      fw->rays[i].y += fw->rays[i].sy;
    }
  }
  fw->R = fw->Y - fw->rays[0].y;
  if ((fw->R > R0 / 2) && !fw->dissolving) {
    fw->dissolving = true;
    erase_radius = R0 / 3;
  }
  fw->growing = finished < NUM_RAYS; // All rays finished?
}

void StepDissolve(Firework* fw) {
  // One step of dissolving the fireworks[0]
  // Start dissolving when fireworks[0] is half done

#ifdef DEV_DEB
  Platform::DebugPrint((uint8_t*)"erase_radius=");
  Platform::DebugPrint((uint16_t)erase_radius);
  Platform::DebugPrint((uint8_t*)"fw->R=");
  Platform::DebugPrint((uint16_t)fw->R, true);
#endif
  if (fw->growing) {
    DrawScatterDisk(fw->X, fw->Y, fw->R, 2, COLOUR_BLACK);
  } else {
    Platform::DrawFilledCircle(fw->X, fw->Y, erase_radius, COLOUR_BLACK);
    erase_radius += 4;
    if (erase_radius > R0) {
      fw->dissolving = false;
      fw->R = 0; // Reset for reuse
    }
  }
  modified = true;
}

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
