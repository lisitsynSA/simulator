#include "life.h"
#include <QRandomGenerator>
#include <QTimer>

Life::Life(uint32_t xSize, uint32_t ySize, QObject *parent) :
    QObject(parent), m_xSize(xSize), m_ySize(ySize), m_ym(xSize + 2) {
    m_space = new uint32_t[xSize*ySize];
    m_tmp = new uint8_t[m_ym*(ySize + 2)];
    m_tmpNew = new uint8_t[m_ym*(ySize + 2)];
    generateSpace();
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(playGame()));
}

Life::~Life() {
    delete m_tmpNew;
    delete m_tmp;
    delete m_space;
}

void Life::generateSpace() {
    for (uint32_t y = 1; y <= m_ySize; y++)
    for (uint32_t x = 1; x <= m_xSize; x++) {
        if (QRandomGenerator::global()->bounded(2)) {
            // alive
            m_tmp[y*m_ym + x] = 1;
        } else {
            // dead
            m_tmp[y*m_ym + x] = 0;
        }
    }
}

void Life::startGame() {
    m_timer->start(100);
}

void Life::playGame() {
    calcSpace();
    drawSpace();
    emit sendSpace(m_space, m_xSize, m_ySize);
}

void Life::calcSpace() {
    for (uint32_t y = 1; y <= m_ySize; y++)
    for (uint32_t x = 1; x <= m_xSize; x++) {
        m_tmpNew[y*m_ym + x] = calcCell(x, y);
    }
    for (uint32_t y = 1; y <= m_ySize; y++) {
        m_tmpNew[y*m_ym] = m_tmpNew[y*m_ym + m_xSize];
        m_tmpNew[y*m_ym + m_xSize + 1] = m_tmpNew[y*m_ym + 1];
    }
    for (uint32_t x = 1; x <= m_xSize; x++) {
        m_tmpNew[x] = m_tmpNew[m_ySize*m_ym + x];
        m_tmpNew[(m_ySize + 1)*m_ym + x] = m_tmpNew[m_ym + x];
    }
    m_tmpNew[0] = m_tmpNew[m_ySize*m_ym + m_xSize];
    m_tmpNew[(m_ySize + 1)*m_ym + m_xSize + 1] = m_tmpNew[m_ym + 1];
    m_tmpNew[m_xSize + 1] = m_tmpNew[m_ySize*m_ym + 1];
    m_tmpNew[(m_ySize + 1)*m_ym] = m_tmpNew[m_ym + m_xSize];

    std::swap(m_tmpNew, m_tmp);
}

uint8_t Life::calcCell(uint32_t x, uint32_t y) {
    uint32_t x_l = x - 1;
    uint32_t x_r = x + 1;
    uint32_t y_u = y - 1;
    uint32_t y_d = y + 1;
    uint32_t res = 0;
    res += m_tmp[y_u*m_ym + x_l] + m_tmp[y_u*m_ym + x] + m_tmp[y_u*m_ym + x_r] +
           m_tmp[y  *m_ym + x_l] + m_tmp[y  *m_ym + x] + m_tmp[y  *m_ym + x_r] +
           m_tmp[y_d*m_ym + x_l] + m_tmp[y_d*m_ym + x] + m_tmp[y_d*m_ym + x_r];

    if (res == 3 || (res == 4 && m_tmp[y*m_ym + x])) {
        return 1;
    }
    return 0;
}

void Life::drawSpace() {
    for (uint32_t y = 1; y <= m_ySize; y++)
    for (uint32_t x = 1; x <= m_xSize; x++) {
        uint32_t &color = m_space[(y - 1)*m_xSize + x - 1];
        uint8_t &cell = m_tmp[y*m_ym + x];
        if (cell) {
            color |= 0xff00ff00;
            if ((color & 0xff) != 0xff) {
                color++;
            }
        } else {
            color = 0xff000000;
        }
    }
}
