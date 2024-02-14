#include <stdint.h>
#include "platform.h"
#include "utils.h"

uint16_t r = 54167; // Default seed

uint16_t Rand() {
  r = 77 * r + 73;
  return r;
}

void SRand(uint16_t seed) {
  r = seed;
}
