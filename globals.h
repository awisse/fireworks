// Wall, Target, Box, Box on Target
#pragma once
#include "defines.h"
/* typedef enum Piece Piece; */
typedef enum {
  Short_A,
  Long_A,
  Short_B,
  Long_B,
  /* VeryLong_B, */
  Left,
  Right,
  Up,
  Down
} Event;

enum Direction {
  down,
  left,
  right,
  up
};

typedef struct {
  int8_t x; // Drawing coordinates
  int8_t y; // Drawing coordinates
  int8_t x0; // Generic ray endpoint
  int8_t y0; // Generic ray endpoint
  int8_t x1; // Translated ray endpoint
  int8_t y1; // Translated ray endpoint
  int8_t dx;
  int8_t dy;
  int8_t sx;
  int8_t sy;
  int8_t err;
} Ray;

typedef struct {
  Ray rays[NUM_RAYS];
  int8_t X; // Center coordinates
  int8_t Y; // Center coordinates
  int8_t R;
  bool growing;
  bool dissolving;
} Firework;

constexpr int8_t R0 = RAY_LENGTH;
// vim:ft=cpp
