#include "locus.h"

void Locus::drawSpace(uint32_t *space) {
    for (uint32_t point: m_points){
        space[point] = m_color;
    }
}

void Locus::relax() {
    if (m_points.empty()) {
        return;
    }
    m_x = 0;
    m_y = 0;
    for (uint32_t point: m_points){
        m_x += point % m_size;
        m_y += point / m_size;
    }
    m_x /= m_points.size();
    m_y /= m_points.size();
}

void Locus::relaxTor() {
    if (m_points.empty()) {
        return;
    }
    m_x = 0;
    m_y = 0;
    for (uint32_t point: m_points){
        m_x += point % m_size;
        m_y += point / m_size;
    }
    m_x /= m_points.size();
    m_y /= m_points.size();
}
