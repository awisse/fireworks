// Drawing/display functions
#pragma once
#include <stdint.h>
#include "globals.h"
#include "defines.h"

// Sprite indexes


// Local Functions
void DrawLine(int8_t x0, int8_t y0, int8_t x1, int8_t y1);
void DrawStar(Star* star);
void DrawScatterRect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, 
    uint8_t bits, uint8_t colour);
void DrawScatterDisk(uint8_t x0, uint8_t y0, uint8_t r, uint8_t bits, 
    uint8_t colour);
void Dissolve(int8_t x, int8_t y, uint8_t r);
//
// vim:ft=cpp
