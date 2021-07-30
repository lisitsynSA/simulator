#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "display.h"

Display::Display(QGraphicsView* view, QObject *parent)
    : QObject(parent), m_view(view)
{
    m_scene = new QGraphicsScene(this);
    uint32_t array[1000*1000];
    for (int i = 0; i < 1000*1000; i++) {
        array[i] = 0xff000000 + i;
    }
    m_image = new QImage((uchar*)array, 1000, 1000, QImage::Format_ARGB32);
    m_item = new QGraphicsPixmapItem(QPixmap::fromImage(*m_image));

    m_scene->addItem(m_item);

    m_view->setScene(m_scene);
}
