#include "mapgenerator.h"
#include "locus.h"
#include "relaxationtor.h"
#include <QDebug>

MapGenerator::MapGenerator(uint32_t xSize, uint32_t ySize, QObject *parent) :
    QObject(parent), m_xSize(xSize), m_ySize(ySize)
{
    m_space = new uint32_t[xSize*ySize];
}

MapGenerator::~MapGenerator() {
    delete m_space;
}

void MapGenerator::generateLocuses() {
    RelaxationTor relaxTor(m_xSize, m_ySize, this);
    relaxTor.generateLocuses();
    relaxTor.relax();
    relaxTor.relax();
    relaxTor.copyLocuses(&m_locuses);
    for (Locus& locus: m_locuses) {
        for (uint32_t& point: locus.m_points) {
            m_locusMap[point] = &locus;
        }
    }
    drawSpace();
    emit sendSpace(m_space, m_xSize, m_ySize);
}

void MapGenerator::generateHeight() {
    for(Locus& locus: m_locuses) {
        locus.m_z = qrand() & 0xff;
        locus.calcColor();
    }
    drawSpace();
    emit sendSpace(m_space, m_xSize, m_ySize);
}

void MapGenerator::select(uint32_t x, uint32_t y) {
    Locus* locus = m_locusMap[y*m_xSize + x];
    uint32_t oldColor = locus->m_color;
    locus->m_color = 0xffff0000;
    locus->drawSpace(m_space);
    emit sendSpace(m_space, m_xSize, m_ySize);
    locus->m_color = oldColor;
}

void MapGenerator::drawSpace() {
    for(Locus& locus: m_locuses) {
        locus.drawSpace(m_space);
    }
}
