#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;

class Display : public QObject
{
    Q_OBJECT
public:
    Display(QGraphicsView* view, QObject *parent = nullptr);
public slots:
    void clean();
    void loadARGB32(uint32_t *data, uint32_t width, uint32_t height);

signals:

private:
    QGraphicsView* m_view = nullptr;
    QGraphicsScene* m_scene = nullptr;
    QGraphicsPixmapItem* m_item = nullptr;
};

#endif // DISPLAY_H
