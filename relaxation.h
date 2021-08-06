#ifndef RELAXATION_H
#define RELAXATION_H

#include <QObject>
class QTimer;
class Locus;

class Relaxation : public QObject
{
    Q_OBJECT
public:
    explicit Relaxation(uint32_t xSize, uint32_t ySize, QObject *parent = nullptr);
    ~Relaxation();
    void copyLocuses(std::vector<Locus> *locuses);
    void generateLocuses();
    virtual void relax();
public slots:
    virtual void startRelaxation(int32_t repeat = -1);
    virtual void stopRelaxation();
    void play();
signals:
    void sendSpace(uint32_t* space, uint32_t width, uint32_t height);
    void finishRelax();
protected:
    uint32_t m_xSize;
    uint32_t m_ySize;
    uint32_t *m_space;
    QTimer *m_timer;
    int32_t m_repeat;
    std::vector<Locus> m_locuses;

    void drawSpace();
    void gatherPoints();
    virtual uint32_t dist(Locus &locus, uint32_t x, uint32_t y);
};

#endif // RELAXATION_H
