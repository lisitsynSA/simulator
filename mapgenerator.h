#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "relaxationtor.h"
class Locus;
class RelaxationTor;

class MapGenerator : public RelaxationTor
{
    Q_OBJECT
public:
    explicit MapGenerator(uint32_t xSize, uint32_t ySize, QObject *parent = nullptr);
public slots:
    void generateHeight(int32_t repeat = 10);
    void heightRelaxation();
    void select(uint32_t x, uint32_t y);
    virtual void stopRelaxation();
    virtual void startRelaxation(int32_t repeat = 5);
protected:
    std::map<uint32_t, Locus*> m_locusMap;
    QTimer *m_hTimer;

    void findNeighbors(Locus &locus);
    void addMapRand();
};

#endif // MAPGENERATOR_H
