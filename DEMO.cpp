#include "DEMO_interface.h"

#define GAS_SIZE 128
#define V_MAX 16
#define SLOWDOWN 4 // (V_MAX - SLOWDOWN) / V_MAX
#define SCALE 8
#define SIZE_X SCALE *DEMO_SIZE_X
#define SIZE_Y SCALE *DEMO_SIZE_Y

typedef struct {
  int32_t x;
  int32_t y;
  int32_t vx;
  int32_t vy;
} mol_t;

mol_t gas[GAS_SIZE];

void init_gas() {
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    gas[i].x = DEMO_rand() % (SCALE * DEMO_SIZE_X);
    gas[i].y = DEMO_rand() % (SCALE * DEMO_SIZE_Y);
    gas[i].vx = (DEMO_rand() % (2 * V_MAX)) - V_MAX;
    gas[i].vy = (DEMO_rand() % (2 * V_MAX)) - V_MAX;
  }
  for (uint32_t y = 0; y < DEMO_SIZE_Y; y++)
    for (uint32_t x = 0; x < DEMO_SIZE_X; x++) {
      DEMO_put_pixel(y, x, 0xff000000);
    }
}

uint32_t get_mol_color(uint32_t vx, uint32_t vy, int32_t v2max) {
  uint32_t v = vx * vx + vy * vy;
  uint32_t red = 0xff * v / v2max;
  uint32_t blue = 0xff - red;
  return 0xff000000 + (red << 16) + blue;
}

void draw_gas() {
  int32_t v2max = 0;
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    if (v2max < gas[i].vx * gas[i].vx + gas[i].vy * gas[i].vy) {
      v2max = gas[i].vx * gas[i].vx + gas[i].vy * gas[i].vy;
    }
  }
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    DEMO_put_pixel(gas[i].y / SCALE, gas[i].x / SCALE,
                   get_mol_color(gas[i].vx, gas[i].vy, v2max));
  }
  DEMO_flush();
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    DEMO_put_pixel(gas[i].y / SCALE, gas[i].x / SCALE, 0xff000000);
  }
}

void step_gas() {
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    gas[i].x += gas[i].vx;
    if (gas[i].x < 0) {
      gas[i].x = -gas[i].x;
      gas[i].vx = -gas[i].vx;
    }
    if (gas[i].x > SIZE_X) {
      gas[i].x = 2 * SIZE_X - gas[i].x;
      gas[i].vx = -gas[i].vx;
    }
    gas[i].y += gas[i].vy;
    if (gas[i].y < 0) {
      gas[i].y = -gas[i].y;
      gas[i].vy = -gas[i].vy;
    }
    if (gas[i].y > SIZE_Y) {
      gas[i].y = 2 * SIZE_Y - gas[i].y;
      gas[i].vy = -gas[i].vy;
    }
  }
}

void calc_gas() {
  // Collision
  for (uint32_t mol = 0; mol < GAS_SIZE; mol++) {
    for (uint32_t mol2 = 0; mol2 < GAS_SIZE; mol2++) {
      if (mol == mol2) {
        continue;
      }
      int32_t distx = gas[mol].x - gas[mol2].x;
      int32_t disty = gas[mol].y - gas[mol2].y;
      if ((distx * distx + disty * disty) < SCALE * SCALE) {
        uint32_t aver_x = (gas[mol].x + gas[mol2].x) / 2;
        if ((gas[mol].x + gas[mol2].x) % 2 && DEMO_rand() % 2) {
          aver_x += 1;
        }
        uint32_t aver_y = (gas[mol].y + gas[mol2].y) / 2;
        if ((gas[mol].y + gas[mol2].y) % 2 && DEMO_rand() % 2) {
          aver_y += 1;
        }
        if (gas[mol2].x < gas[mol].x) {
          gas[mol2].x = aver_x - SCALE / 2;
          gas[mol].x = aver_x + SCALE / 2;
        } else {
          gas[mol].x = aver_x - SCALE / 2;
          gas[mol2].x = aver_x + SCALE / 2;
        }
        if (gas[mol2].y < gas[mol].y) {
          gas[mol2].y = aver_y - SCALE / 2;
          gas[mol].y = aver_y + SCALE / 2;
        } else {
          gas[mol].y = aver_y - SCALE / 2;
          gas[mol2].y = aver_y + SCALE / 2;
        }
        int32_t tmp_vx = gas[mol].vx * (V_MAX - SLOWDOWN) / V_MAX;
        gas[mol].vx = gas[mol2].vx * (V_MAX - SLOWDOWN) / V_MAX;
        gas[mol2].vx = tmp_vx;
        int32_t tmp_vy = gas[mol].vy * (V_MAX - SLOWDOWN) / V_MAX;
        gas[mol].vy = gas[mol2].vy * (V_MAX - SLOWDOWN) / V_MAX;
        gas[mol2].vy = tmp_vy;
      }
    }
  }

  // Gravity
  for (uint32_t mol = 0; mol < GAS_SIZE; mol++) {
    int32_t fx = 0;
    int32_t fy = 0;
    for (uint32_t mol2 = 0; mol2 < GAS_SIZE; mol2++) {
      if (mol == mol2) {
        continue;
      }

      int32_t distx = gas[mol2].x - gas[mol].x;
      if (0 < distx && distx < SCALE) {
        distx = SCALE;
      }
      if (-SCALE < distx && distx < 0) {
        distx = -SCALE;
      }
      int32_t disty = gas[mol2].y - gas[mol].y;
      if (0 < disty && disty < SCALE) {
        disty = SCALE;
      }
      if (-SCALE < disty && disty < 0) {
        disty = -SCALE;
      }
      int32_t dist2 = (distx * distx + disty * disty) / (SCALE * SCALE);
      if (dist2 < 2) {
        dist2 = 2;
      }
      if (distx != 0)
        fx += V_MAX / (dist2 * distx);
      if (disty != 0)
        fy += V_MAX / (dist2 * disty);
    }
    gas[mol].vx += fx;
    gas[mol].vy += fy;
  }
}

void DEMO_main() {
  init_gas();
  while (1) {
    draw_gas();
    calc_gas();
    step_gas();
  }
}
