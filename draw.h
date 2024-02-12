// Drawing/display functions
#pragma once
#include <stdint.h>
#include "globals.h"
#include "defines.h"

// Sprite indexes


// Local Functions
void DrawLine(int8_t x0, int8_t y0, int8_t x1, int8_t y1);
void DrawStar(Ray star[NUM_RAYS]);
void Dissolve(uint8_t x, uint8_t y, uint8_t r);
//
// vim:ft=cpp
