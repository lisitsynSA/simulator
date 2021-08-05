#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <QObject>
class Locus;

class MapGenerator : public QObject
{
    Q_OBJECT
public:
    explicit MapGenerator(uint32_t xSize, uint32_t ySize, QObject *parent = nullptr);
    ~MapGenerator();
public slots:
    void generateLocuses();
    void generateHeight();
    void select(uint32_t x, uint32_t y);
signals:
    void sendSpace(uint32_t* space, uint32_t width, uint32_t height);
protected:
    uint32_t m_xSize;
    uint32_t m_ySize;
    uint32_t *m_space;
    std::vector<Locus> m_locuses;
    std::map<uint32_t, Locus*> m_locusMap;

    void drawSpace();
    void findNeighbors(Locus &locus);
    uint32_t dist(Locus &locus1, Locus &locus2);
};

#endif // MAPGENERATOR_H
