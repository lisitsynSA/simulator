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
    void loadARGB32(uchar *data, int width, int height);

signals:

private:
    QGraphicsView* m_view = nullptr;
    QGraphicsScene* m_scene = nullptr;
    QGraphicsPixmapItem* m_item = nullptr;
    QImage* m_image = nullptr;
};

#endif // DISPLAY_H
