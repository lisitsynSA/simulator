#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "display.h"

void mousePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Custom view clicked: " << event->scenePos();
    emit m_displ->click(event->scenePos().x(), event->scenePos().y());
    QGraphicsItem::mousePressEvent(event);
}

Display::Display(QWidget *parent)
    : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    m_item = new mousePixmapItem(this);

    m_scene->addItem(m_item);
    setScene(m_scene);
}

void Display::clean() {
    m_item->setPixmap(QPixmap());
}

void Display::loadARGB32(uint32_t *data, uint32_t width, uint32_t height) {
    m_item->setPixmap(QPixmap::fromImage(QImage((uchar*) data, width, height, QImage::Format_ARGB32)));
}
