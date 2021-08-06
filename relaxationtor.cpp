#include "relaxationtor.h"
#include "locus.h"

void RelaxationTor::relax() {
    for(Locus& locus: m_locuses) {
        locus.relaxTor();
        locus.clear();
    }
    //gatherPoints();
    gatherPointsFast();
}

uint32_t RelaxationTor::dist(Locus &locus, uint32_t x, uint32_t y) {
    uint32_t x1 = std::min(locus.m_x, x);
    uint32_t x2 = std::max(locus.m_x, x);
    uint32_t y1 = std::min(locus.m_y, y);
    uint32_t y2 = std::max(locus.m_y, y);
    uint32_t x_midd = x2 - x1;
    uint32_t x_edge = m_xSize - x_midd;
    uint32_t y_midd = y2 - y1;
    uint32_t y_edge = m_ySize - y_midd;
    uint32_t xRes = std::min(x_midd, x_edge);
    uint32_t yRes = std::min(y_midd, y_edge);
    return xRes*xRes + yRes*yRes;
}


bool RelaxationTor::drawLocusEdge(Locus *locus, uint32_t len) {
    bool changed = false;
    for (uint32_t i = 0; i <= len; i++) {
        int32_t x_l = locus->m_x - i;
        if (x_l < 0) {
            x_l += m_xSize;
        }
        int32_t x_r = locus->m_x + i;
        if (x_r >= (int32_t)m_xSize) {
            x_r -= m_xSize;
        }
        int32_t y_u = locus->m_y - len + i;
        if (y_u < 0) {
            y_u += m_ySize;
        }
        int32_t y_d = locus->m_y + len - i;
        if (y_d >= (int32_t)m_ySize) {
            y_d -= m_ySize;
        }
        if (m_space[y_u*m_xSize + x_l] == 0) {
            m_space[y_u*m_xSize + x_l] = locus->m_color;
            locus->add(y_u*m_xSize + x_l);
            changed = true;
        }
        if (m_space[y_d*m_xSize + x_l] == 0) {
            m_space[y_d*m_xSize + x_l] = locus->m_color;
            locus->add(y_d*m_xSize + x_l);
            changed = true;
        }
        if (m_space[y_u*m_xSize + x_r] == 0) {
            m_space[y_u*m_xSize + x_r] = locus->m_color;
            locus->add(y_u*m_xSize + x_r);
            changed = true;
        }
        if (m_space[y_d*m_xSize + x_r] == 0) {
            m_space[y_d*m_xSize + x_r] = locus->m_color;
            locus->add(y_d*m_xSize + x_r);
            changed = true;
        }
    }
    return changed;
}
