#include "defines.h"
/* #include "globals.h" */
#include "draw.h"
#include "platform.h"
#include "font.h"

Font font;
uint16_t displayed_lvl = 1; // Level displayed in upper left corner

uint8_t len(const uint8_t* text) {
  // Compute length of utf-8 string
  uint8_t length=0;

  while (*text++) {
    if ((*text & 0xC0) != 0xC0)
      length++;
  }
  return length;
}

void Draw() {

}

void DrawLine(int8_t x0, int8_t y0, int8_t x1, int8_t y1) {

  int8_t dx = ABSDIFF(x1,x0);
  int8_t sx = SIGNXY(x0,x1);
  int8_t dy = ABSDIFF(y1,y0);
  int8_t sy = SIGNXY(y0,y1);
  int8_t err = (dx>dy ? dx : -dy)/2;
  int8_t e2;

  for(;;){
    Platform::PutPixel(x0, y0, COLOUR_WHITE);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void DrawStar(Ray star[NUM_RAYS]) {
  // Draw a star with 12 rays simultaneously from one ray
  int8_t i;

  for (i=0; i<NUM_RAYS; i++) {
    DrawLine(star[i].x, star[i].y, star[i].x1, star[i].y1);
  }
}

void Dissolve(uint8_t x, uint8_t y, uint8_t r) {
  uint16_t ix;
  uint8_t i;

  for (i=0; i<DISSOLVE_PX; i++) {
    ix = Platform::Random(0, (x + r) * (y + r) / 8);
    Platform::PutPixel(ix & 0xFF, ix >> 4, COLOUR_BLACK);
  }
}
//
// vim:fdm=syntax
