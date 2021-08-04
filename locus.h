#ifndef LOCUS_H
#define LOCUS_H

#include <QObject>

class Locus
{
public:
    Locus(uint32_t x, uint32_t y, uint32_t color, uint32_t size) :
        m_x(x), m_y(y), m_color(color), m_size(size) {}
    void drawSpace(uint32_t *space);
    void relax();
    void relaxTor();
    void clear() {
        m_points.clear();
    }
    void add(uint32_t point) {
        m_points.push_back(point);
    }
    uint32_t getX() { return m_x; }
    uint32_t getY() { return m_y; }
    uint32_t getColor() { return m_color; }
private:
    uint32_t m_x;
    uint32_t m_y;
    uint32_t m_color;
    uint32_t m_size;
    std::vector<uint32_t> m_points;
};

#endif // LOCUS_H
