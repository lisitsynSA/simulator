#ifndef LIFE_H
#define LIFE_H

#include <QObject>
class QTimer;
class Life : public QObject
{
    Q_OBJECT
public:
    explicit Life(uint32_t xSize, uint32_t ySize, QObject *parent = nullptr);
    ~Life();
public slots:
    void startGame();
    void addVirus();
    void addVirus(uint32_t x, uint32_t y);
    void playGame();
signals:
    void sendSpace(uint32_t* space, uint32_t width, uint32_t height);
private:
    uint32_t m_xSize;
    uint32_t m_ySize;
    uint32_t m_ym;
    uint32_t *m_space;
    uint8_t *m_tmp;
    uint8_t *m_tmpNew;
    QTimer *m_timer;
    uint32_t m_virusQueue;

    void generateSpace();
    void calcSpace();
    uint8_t calcCell(uint32_t x, uint32_t y);
    void drawSpace();
};

#endif // LIFE_H
