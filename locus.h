#ifndef LOCUS_H
#define LOCUS_H

#include <QObject>

class Locus {
public:
  Locus(uint32_t x, uint32_t y, uint32_t color, uint32_t xSize, uint32_t ySize)
      : m_x(x), m_y(y), m_z(0), m_color(color), m_xSize(xSize), m_ySize(ySize) {
  }
  void drawSpace(uint32_t *space);
  void relax();
  void relaxTor();
  void clear() { m_points.clear(); }
  void add(uint32_t point) { m_points.push_back(point); }
  void calcHeightColor();
  void addNeighbor(Locus *locus);
  void averageZ();
  Locus *minNeighbor();
  Locus *minNonRiver();
  uint32_t m_x;
  uint32_t m_y;
  uint32_t m_z;
  uint32_t m_color;
  uint32_t m_xSize;
  uint32_t m_ySize;
  std::vector<uint32_t> m_points;
  std::vector<Locus *> m_neighbors;
  bool m_fixZ = false;
  bool m_river = false;
};

#endif // LOCUS_H
