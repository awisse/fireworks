#pragma once

// Platform dependant definitions
#ifndef ARDUINO
#include <stdint.h>
#include <string.h>
#define PROGMEM
#define PSTR
#define pgm_read_byte(x) *(x)
#define pgm_read_word(x) (*((uint16_t*)x))
#define pgm_read_ptr(x) (*((uintptr_t*)x))
#define strlen_P(x) strlen(x)
#else
#include <avr/pgmspace.h>
//#define pgm_read_ptr pgm_read_word
#endif

/* #define _DEBUG */

// Game Parameters
#define FRAME_RATE 40
#define MAX_FIREWORKS 6
#define NUM_RAYS 12
#define RAY_LENGTH 24
#define PLACES MAX_FIREWORKS * NUM_STARS
#define DISSOLVE_PX 32

// Mathematical constants
// 128 * cos(PI/6). For 12 Rays.
// For a different number of rays, must define more values
#define COS30 111u

// Functions
#define ABSDIFF(x, y) (x < y) ? y - x : x - y
#define SIGNXY(x0, x1) (x0 < x1) ? 1 : -1

// Conversions
#define U8 (uint8_t*)

// Board Dimensions

// Screen Dimensions
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define COLOUR_BLACK 0
#define COLOUR_WHITE 1
#define INVERT 2

// Buttons
#define INPUT_LEFT 32u
#define INPUT_RIGHT 64u
#define INPUT_UP 128u
#define INPUT_DOWN 16u
#define INPUT_A 8u
#define INPUT_B 4u

// Time
#define LONG_PRESS 2000
