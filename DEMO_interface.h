#ifndef DEMO_INTERFACE_H
#define DEMO_INTERFACE_H
#include <stdint.h>

#define DEMO_SIZE_X 256
#define DEMO_SIZE_Y 128

void DEMO_put_pixel(uint32_t y, uint32_t x, uint32_t argb);
void DEMO_flush();
uint32_t DEMO_rand();

#endif // DEMO_INTERFACE_H
