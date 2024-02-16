#include <Arduboy2.h>
#include "defines.h"
#include "game.h"
#include "platform.h"

Arduboy2Base arduboy;

void setup() {
  // put your setup code here, to run once:

  arduboy.begin();
  arduboy.setFrameRate(FRAME_RATE);

#ifdef DEV_DEB
  Serial.begin(9600);
#endif

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
    if (StepGame());
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
  if ((x < 0) || (y < 0) || (x >= DISPLAY_WIDTH) || (y >= DISPLAY_HEIGHT)) {
    return;
  }
  arduboy.drawPixel(x, y, colour);
}

void Platform::DrawBitmap(const uint8_t* bitmap,  int16_t x, int16_t y, uint8_t w,
                uint8_t h, uint8_t colour) {
  arduboy.drawBitmap(x, y, bitmap, w, h, colour);
}

void Platform::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour) {
  arduboy.drawLine(x0, y0, x1, y1, colour);
}

void Platform::DrawFilledCircle(int16_t x0, int16_t y0, uint8_t r,
    uint8_t colour=COLOUR_WHITE) {
  arduboy.fillCircle(x0, y0, r, colour);
}

void Platform::Clear() {
  arduboy.clear();
}

unsigned long Platform::Millis() {
  return millis();
}

#ifdef DEV_DEB
void Platform::DebugPrint(uint16_t value, bool endl) {
  Serial.print(value);
  Serial.print(':');
  if (endl) {
    Serial.println();
  }
}

void Platform::DebugPrint(int16_t value, bool endl) {
  Serial.print(value);
  Serial.print(':');
  if (endl) {
    Serial.println();
  }
}

void Platform::DebugPrint(float value, bool endl) {
  Serial.print(value);
  Serial.print(':');
  if (endl) {
    Serial.println();
  }
}

void Platform::DebugPrint(const uint8_t* text, bool endl) {
  Serial.print((char*)text);
  if (endl) {
    Serial.println();
  }
}
#endif
// vim:ts=2:sts=2:sw=2:expandtab
