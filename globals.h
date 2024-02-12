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
  int8_t x0;
  int8_t y0;
  int8_t x1;
  int8_t y1;
  int8_t dx;
  int8_t dy;
  int8_t sx;
  int8_t sy;
  int8_t err;
} Ray;


// vim:ft=cpp
