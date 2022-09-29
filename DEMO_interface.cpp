#include "DEMO_interface.h"
#include "display.h"
#include <QRandomGenerator>
#include <QThread>

uint32_t *DEMO_data;
uint32_t data[DEMO_SIZE_X * DEMO_SIZE_Y] = {};
uint32_t buf_data[DEMO_SIZE_X * DEMO_SIZE_Y] = {};

void DEMO_put_pixel(uint32_t y, uint32_t x, uint32_t argb) {
  buf_data[y * DEMO_SIZE_X + x] = argb;
}

void DEMO_flush() {
  memcpy(data, buf_data, DEMO_SIZE_X * DEMO_SIZE_Y * sizeof(uint32_t));
  DEMO_data = data;
  QThread::msleep(10);
}

uint32_t DEMO_rand() { return QRandomGenerator::global()->generate(); }
