#include <Arduboy2.h>
#include <EEPROM.h>
#include "game.h"
#include "controller.h"
#include "defines.h"
#include "font.h"
#include "platform.h"

Arduboy2Base arduboy;
int counter;

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(20);

#ifdef _DEBUG
  Serial.begin(9600);
  counter = 0;
#endif

  InitGame();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (arduboy.nextFrame()) {
    StepGame();
    arduboy.display();
  }
}

// platform.h
// General
uint8_t* Platform::GetBuffer() {
  return arduboy.sBuffer;
}

uint8_t Platform::ButtonState() {
  uint8_t buttons = arduboy.buttonsState();
  return buttons;
  // Debounce
}

// Drawing
void Platform::DrawBuffer() {}

void Platform::PutPixel(uint8_t x, uint8_t y, uint8_t colour) {
  arduboy.drawPixel(x, y, colour);
}

void Platform::DrawBitmap(const uint8_t* bmp, int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t colour) {
  arduboy.drawBitmap(x, y, bmp, w, h, colour);
}

void Platform::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour) {
  arduboy.drawLine(x0, y0, x1, y1, colour);
}

void Platform::DrawRect(int16_t x, int16_t y, uint8_t w, uint8_t h) {
  arduboy.drawRect(x, y, w, h);
}

void Platform::DrawFilledRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t colour) {
  arduboy.fillRect(x, y, w, h, colour);
}

void Platform::DrawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t colour) {
  arduboy.drawCircle(x0, y0, r, colour);
}

void Platform::DrawFilledCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t colour) {
  arduboy.fillCircle(x0, y0, r, colour);
}

void Platform::Clear() {
  arduboy.clear();
}

unsigned long Platform::Millis() {
  return millis();
}

#define EEP(x) EEPROM[EEPROM_STORAGE_SPACE_START + x]

SavedState Platform::ToEEPROM(const uint8_t *bytes, const uint16_t offset,
                    const uint16_t sz) {
  int i;
  if (offset < 0) {
    return WrongOffset;
  }
  if (EEPROM_STORAGE_SPACE_START + offset + sz > EEPROM.length()) {
    return TooBig;
  }
  // Write Game to EEPROM

  for (i = 0; i < sz; i++) {
    EEP(i + offset) = bytes[i];
  }
  return Saved;
}

SavedState Platform::FromEEPROM(uint8_t *bytes, const uint16_t offset,
                      const uint16_t sz) {
  // Get Game from EEPROM
  int i = EEPROM_STORAGE_SPACE_START + offset;
  if (i < 0) {
    return WrongOffset;
  }
  if (i + sz > EEPROM.length()) {
    return TooBig; // Can't read from here
  }

  for (i = 0; i < sz; i++) {
    bytes[i] = EEP(i + offset);
  }
  return Saved;
}

#ifdef _DEBUG
void Platform::DebugPrint(uint16_t value) {
  Serial.print(value);
  if (++counter % 8 == 0) {
    Serial.println();
  }
}

void Platform::DebugPrint(unsigned long value) {
  Serial.print(value);
  if (++counter % 8 == 0) {
    Serial.println();
  }
}

void Platform::DebugPrint(float value) {
  Serial.print(value);
  if (++counter % 8 == 0) {
    Serial.println();
  }
}

void Platform::DebugPrint(const uint8_t* text) {
  Serial.println((char*)text);
}
#endif
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=arduino
