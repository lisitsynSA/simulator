#include "locus.h"

void Locus::drawSpace(uint32_t *space) {
  for (uint32_t point : m_points) {
    space[point] = m_color;
  }
}

void Locus::relax() {
  if (m_points.empty()) {
    return;
  }
  m_x = 0;
  m_y = 0;
  for (uint32_t point : m_points) {
    m_x += point % m_xSize;
    m_y += point / m_xSize;
  }
  m_x /= m_points.size();
  m_y /= m_points.size();
}

void Locus::relaxTor() {
  if (m_points.empty()) {
    return;
  }
  uint32_t x_midd = 0;
  uint32_t x_edge = 0;
  bool xEdged = false;
  uint32_t y_midd = 0;
  uint32_t y_edge = 0;
  bool yEdged = false;
  for (uint32_t point : m_points) {
    uint32_t x = point % m_xSize;
    x_midd += x;
    x_edge += (x < m_xSize / 2) ? x + m_xSize : x;
    if (x == 0) {
      xEdged = true;
    }
    uint32_t y = point / m_xSize;
    y_midd += y;
    y_edge += (y < m_ySize / 2) ? y + m_ySize : y;
    if (y == 0) {
      yEdged = true;
    }
  }
  x_midd /= m_points.size();
  x_edge = (x_edge / m_points.size()) % m_xSize;
  y_midd /= m_points.size();
  y_edge = (y_edge / m_points.size()) % m_ySize;
  m_x = (xEdged) ? x_edge : x_midd;
  m_y = (yEdged) ? y_edge : y_midd;
}

void Locus::addNeighbor(Locus *locus) {
  if (locus == this || locus == nullptr) {
    return;
  }
  for (Locus *neighbor : m_neighbors) {
    if (neighbor == locus) {
      return;
    }
  }
  m_neighbors.push_back(locus);
}

void Locus::averageZ() {
  if (m_fixZ || m_neighbors.empty()) {
    return;
  }
  m_z = 0;
  for (Locus *neighbor : m_neighbors) {
    m_z += neighbor->m_z;
  }
  m_z /= m_neighbors.size();
}

Locus *Locus::minNeighbor() {
  uint32_t minZ = 0xffffffff;
  Locus *minLocus = nullptr;
  for (Locus *neighbor : m_neighbors) {
    if (minZ > neighbor->m_z) {
      minZ = neighbor->m_z;
      minLocus = neighbor;
    }
  }
  return minLocus;
}

Locus *Locus::minNonRiver() {
  uint32_t minZ = 0xffffffff;
  Locus *minLocus = nullptr;
  for (Locus *neighbor : m_neighbors) {
    if (minZ > neighbor->m_z && !neighbor->m_river) {
      minZ = neighbor->m_z;
      minLocus = neighbor;
    }
  }
  return minLocus;
}

void Locus::calcHeightColor() {
  m_color = 0xff000000 + m_z;
  if (m_river) {
    m_color += m_z * 0x100;
  }
}
