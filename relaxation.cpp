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
        if (qrand() % (m_ySize/2 + m_xSize/2) == 0) {
            // locus
            uint32_t color = 0xff000000 + (qrand() & 0xffffff);
            m_locuses.push_back(Locus(x, y, color, m_xSize, m_ySize));
        }
    }
    gatherPoints();
}


void Relaxation::copyLocuses(std::vector<Locus> *locuses) {
    for(Locus& locus: m_locuses) {
        locuses->push_back(locus);
    }
}

void Relaxation::startRelaxation() {
    m_timer->stop();
    generateLocuses();
    m_timer->start(100);
}

void Relaxation::stopRelaxation() {
    m_timer->stop();
}

void Relaxation::play() {
    drawSpace();
    emit sendSpace(m_space, m_xSize, m_ySize);
    relax();
}

void Relaxation::relax() {
    for(Locus& locus: m_locuses) {
        locus.relax();
        locus.clear();
    }
    gatherPoints();
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

uint32_t Relaxation::dist(Locus &locus, uint32_t x, uint32_t y) {
    int32_t xRes = locus.m_x - x;
    int32_t yRes = locus.m_y - y;
    return xRes*xRes + yRes*yRes;
}
