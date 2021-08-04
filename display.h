#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
class QGraphicsScene;
class Display;

class mousePixmapItem : public QGraphicsPixmapItem
{
public:
    mousePixmapItem(Display *display, QGraphicsItem *parent = nullptr):
        QGraphicsPixmapItem(parent), m_displ(display)
    {}
private:
    Display *m_displ;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

class Display : public QGraphicsView
{
    Q_OBJECT
public:
    Display(QWidget *parent = nullptr);
public slots:
    void clean();
    void loadARGB32(uint32_t *data, uint32_t width, uint32_t height);

signals:
    void leftClick(uint32_t x, uint32_t y);
    void rightClick(uint32_t x, uint32_t y);
private:
    QGraphicsScene* m_scene = nullptr;
    mousePixmapItem* m_item = nullptr;
};

#endif // DISPLAY_H
