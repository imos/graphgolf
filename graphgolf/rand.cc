#include <stdint.h>
#include <time.h>
#include <unistd.h>

uint32_t Rand() { 
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = getpid();
  static uint32_t w = time(NULL);
  uint32_t t;
 
  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)); 
}
