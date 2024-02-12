#include <Arduboy2.h>
#include "defines.h"
#include "game.h"

Arduboy2Base arduboy;

void setup() {
  // put your setup code here, to run once:

  arduboy.begin();
  arduboy.setFrameRate(FRAME_RATE);

  while (!arduboy.pressed(A_BUTTON)) {
    arduboy.idle();
  }

  // We need the delay before the first button press for a good seed
  arduboy.initRandomSeed();
  InitGame();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (arduboy.nextFrame()) {
    StepGame();
    arduboy.display();
  }
}

int16_t Platform::Random(int16_t i0, int16_t i1) {

  if (i0 >= i1) return i0;
  return random(i0, i1);
}

// For Controller
uint8_t Platform::ButtonState() {
  return arduboy.buttonsState();
}

// For Drawing
void Platform::PutPixel(uint8_t x, uint8_t y, uint8_t colour)
{
  arduboy.drawPixel(x, y, colour);
}

void Platform::DrawBitmap(const uint8_t* bitmap,  int16_t x, int16_t y, uint8_t w,
                uint8_t h, uint8_t colour) {
  arduboy.drawBitmap(x, y, bitmap, w, h, colour);
}

void Platform::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour) {
  arduboy.drawLine(x0, y0, x1, y1, colour);
}

void Platform::Clear() {
  arduboy.clear();
}

// vim:ts=2:sts=2:sw=2:expandtab
