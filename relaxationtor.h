#ifndef RELAXATIONTOR_H
#define RELAXATIONTOR_H

#include "relaxation.h"

class RelaxationTor : public Relaxation
{
    Q_OBJECT
public:
    RelaxationTor(uint32_t xSize, uint32_t ySize, QObject *parent = nullptr) :
        Relaxation(xSize, ySize, parent) {}
    void relax();
private:
    uint32_t dist(Locus &locus, uint32_t x, uint32_t y);
};

#endif // RELAXATIONTOR_H
