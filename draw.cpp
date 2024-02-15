#include "defines.h"
#include "draw.h"
#include "platform.h"
#include "utils.h"

uint16_t displayed_lvl = 1; // Level displayed in upper left corner

void Dissolve(int8_t x, int8_t y, uint8_t r) {

  Platform::DrawFilledCircle(x, y, r, COLOUR_BLACK);
}

void DrawScatterRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bits, uint8_t colour) {
  // Draw a rectangle with randomly scattered points
  // Bits: Right shift of total number of points:
  //       0: w * h points,
  //       1: (w * h) >> 1 points (50% of all possible points).
  //       2: (w * h) >> 2 points (25% of all possible points).
  //       ...
  uint16_t i; // Loop variable
  uint16_t r; // random variable
  uint16_t dx, dy; // Offset from x0, y0

  for (i=0; i < ((w * h) >> bits); i++) {
    r = Rand();

    dx = ((r >> 8) * w) >> 8;
    dy = ((r & 0xFF) * h) >> 8;

    Platform::PutPixel(x + dx, y + dy, colour);

  }
}

void DrawScatterDisk(uint8_t x0, uint8_t y0, uint8_t r, uint8_t bits, uint8_t colour) {
  uint16_t i; // Loop variable
  uint16_t rv; // random variable
  int16_t dx, dy; // Offset from x0, y0

  for (i=0; i < ((4 * r * r) >> bits); i++) {
    rv = Rand();

    dx = (((rv >> 7)  * r) >> 8) - r;
    dy = (((rv & 0xFF) * 2 * r) >> 8) - r;

    // Pythagoras: Are we inside the circle?
    uint16_t r2 = r * r; // Must transform to 16 bit value for comparison
    if (dx * dx + dy * dy > r2) continue;
#if 0
    uint16_t d2 = dx * dx + dy * dy;
    Platform::DebugPrint((uint8_t*)"dx:dy:dx^2+dy^2:r^2:");
    Platform::DebugPrint(dx);
    Platform::DebugPrint(dy);
    Platform::DebugPrint(d2);
    Platform::DebugPrint(r2, true);
#endif

    Platform::PutPixel(x0 + dx, y0 + dy, colour);
  }
}

//
// vim:fdm=syntax
