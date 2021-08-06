#include "relaxation.h"
#include "locus.h"
#include <QTimer>

Relaxation::Relaxation(uint32_t xSize, uint32_t ySize, QObject *parent) :
    QObject(parent), m_xSize(xSize), m_ySize(ySize)
{
    m_space = new uint32_t[xSize*ySize];
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(play()));
}

Relaxation::~Relaxation() {
    delete m_space;
}

void Relaxation::generateLocuses() {
    m_locuses.clear();
    for (uint32_t y = 0; y < m_ySize; y++)
    for (uint32_t x = 0; x < m_xSize; x++) {
        if (qrand() % (m_ySize + m_xSize)/5 == 0) {
            // locus
            uint32_t color = 0xff000000 + (qrand() & 0xffffff);
            m_locuses.push_back(Locus(x, y, color, m_xSize, m_ySize));
        }
    }
    //gatherPoints();
    gatherPointsFast();
}


void Relaxation::copyLocuses(std::vector<Locus> *locuses) {
    for(Locus& locus: m_locuses) {
        locuses->push_back(locus);
    }
}

void Relaxation::startRelaxation(int32_t repeat) {
    m_timer->stop();
    generateLocuses();
    m_repeat = repeat;
    m_timer->start(100);
}

void Relaxation::stopRelaxation() {
    m_timer->stop();
    emit finishRelax();
}

void Relaxation::play() {
    drawSpace();
    emit sendSpace(m_space, m_xSize, m_ySize);
    relax();
    if (m_repeat != -1) {
        m_repeat--;
        if (m_repeat == 0) {
            stopRelaxation();
        }
    }
}

void Relaxation::relax() {
    for(Locus& locus: m_locuses) {
        locus.relax();
        locus.clear();
    }
    //gatherPoints();
    gatherPointsFast();
}

void Relaxation::drawSpace() {
    for(Locus& locus: m_locuses) {
        locus.drawSpace(m_space);
    }
}

void Relaxation::gatherPoints() {
    for (uint32_t y = 0; y < m_ySize; y++)
    for (uint32_t x = 0; x < m_xSize; x++) {
        uint32_t min = m_ySize*m_ySize + m_xSize*m_xSize;
        Locus *resLocus = nullptr;
        for(Locus& locus: m_locuses) {
            uint32_t distance = dist(locus, x, y);
            if (min > distance) {
                resLocus = &locus;
                min = distance;
            }
        }
        resLocus->add(y*m_xSize + x);
    }
}

void Relaxation::gatherPointsFast() {
    for (uint32_t i = 0; i < m_ySize*m_xSize; i++) {
        m_space[i] = 0;
    }
    bool changed = true;
    for(Locus& locus: m_locuses) {
        m_space[locus.m_y*m_xSize + locus.m_x] = locus.m_color;
        locus.add(locus.m_y*m_xSize + locus.m_x);
    }
    uint32_t len = 1;
    while (changed) {
        changed = false;
        for(Locus& locus: m_locuses) {
            changed |= drawLocusEdge(&locus, len);
        }
        len++;
    }
}

bool Relaxation::drawLocusEdge(Locus *locus, uint32_t len) {
    bool changed = false;
    for (uint32_t i = 0; i <= len; i++) {
        int32_t x_l = locus->m_x - i;
        int32_t x_r = locus->m_x + i;
        int32_t y_u = locus->m_y - len + i;
        int32_t y_d = locus->m_y + len - i;
        if (0 <= x_l && x_l < (int32_t)m_xSize &&
            0 <= y_u && y_u < (int32_t)m_ySize &&
                m_space[y_u*m_xSize + x_l] == 0) {
            m_space[y_u*m_xSize + x_l] = locus->m_color;
            locus->add(y_u*m_xSize + x_l);
            changed = true;
        }
        if (0 <= x_l && x_l < (int32_t)m_xSize &&
            0 <= y_d && y_d < (int32_t)m_ySize &&
                m_space[y_d*m_xSize + x_l] == 0) {
            m_space[y_d*m_xSize + x_l] = locus->m_color;
            locus->add(y_d*m_xSize + x_l);
            changed = true;
        }
        if (0 <= x_r && x_r < (int32_t)m_xSize &&
            0 <= y_u && y_u < (int32_t)m_ySize &&
                m_space[y_u*m_xSize + x_r] == 0) {
            m_space[y_u*m_xSize + x_r] = locus->m_color;
            locus->add(y_u*m_xSize + x_r);
            changed = true;
        }
        if (0 <= x_r && x_r < (int32_t)m_xSize &&
            0 <= y_d && y_d < (int32_t)m_ySize &&
                m_space[y_d*m_xSize + x_r] == 0) {
            m_space[y_d*m_xSize + x_r] = locus->m_color;
            locus->add(y_d*m_xSize + x_r);
            changed = true;
        }
    }
    return changed;
}

uint32_t Relaxation::dist(Locus &locus, uint32_t x, uint32_t y) {
    int32_t xRes = locus.m_x - x;
    int32_t yRes = locus.m_y - y;
    return xRes*xRes + yRes*yRes;
}
