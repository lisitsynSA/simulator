#include "relaxationtor.h"
#include "locus.h"

void RelaxationTor::relax() {
    for(Locus& locus: locuses) {
        locus.relaxTor();
        locus.clear();
    }
    gatherPoints();
}

uint32_t RelaxationTor::dist(Locus &locus, uint32_t x, uint32_t y) {
    uint32_t x1 = std::min(locus.getX(), x);
    uint32_t x2 = std::max(locus.getX(), x);
    uint32_t y1 = std::min(locus.getY(), y);
    uint32_t y2 = std::max(locus.getY(), y);
    uint32_t x_midd = x2 - x1;
    uint32_t x_edge = m_xSize - x_midd;
    uint32_t y_midd = y2 - y1;
    uint32_t y_edge = m_ySize - y_midd;
    uint32_t xRes = std::min(x_midd, x_edge);
    uint32_t yRes = std::min(y_midd, y_edge);
    return xRes*xRes + yRes*yRes;
}
