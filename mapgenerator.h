#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "relaxationtor.h"
class Locus;
class RelaxationTor;

class MapGenerator : public RelaxationTor
{
    Q_OBJECT
public:
    explicit MapGenerator(uint32_t xSize, uint32_t ySize, QObject *parent = nullptr) :
        RelaxationTor(xSize, ySize, parent) {}
public slots:
    void generateHeight();
    void select(uint32_t x, uint32_t y);
    virtual void stopRelaxation();
    virtual void startRelaxation(int32_t repeat = 5);
protected:
    std::map<uint32_t, Locus*> m_locusMap;
    uint32_t m_maxZ;
    uint32_t m_minZ;

    void findNeighbors(Locus &locus);
};

#endif // MAPGENERATOR_H
