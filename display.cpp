#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include "display.h"

Display::Display(QGraphicsView* view, QObject *parent)
    : QObject(parent), m_view(view)
{
    m_scene = new QGraphicsScene(this);
    m_item = new QGraphicsPixmapItem();

    m_scene->addItem(m_item);
    m_view->setScene(m_scene);
}

void Display::clean() {
    m_item->setPixmap(QPixmap());
}

void Display::loadARGB32(uint32_t *data, uint32_t width, uint32_t height) {
    m_item->setPixmap(QPixmap::fromImage(QImage((uchar*) data, width, height, QImage::Format_ARGB32)));
}
