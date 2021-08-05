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
    for (uint32_t point: m_points){
        uint32_t x = point % m_xSize;
        x_midd += x;
        x_edge += (x < m_xSize/2) ? x + m_xSize : x;
        if (x == 0) {
            xEdged = true;
        }
        uint32_t y = point / m_xSize;
        y_midd += y;
        y_edge += (y < m_ySize/2) ? y + m_ySize : y;
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

void Locus::calcColor() {
    if (m_z < 0x20) {
        // water
        m_color = 0xff8080ff;
    } else if (m_z < 0x40) {
        // sand
        m_color = 0xfffff080;
    } else if (m_z < 0xa0) {
        // forest
        m_color = 0xff208000;
    } else if (m_z < 0xe0) {
        // mount
        m_color = 0xff805000;
    } else {
        // snow
        m_color = 0xffffffff;
    }
}
