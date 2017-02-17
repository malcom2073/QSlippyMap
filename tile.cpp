#include "tile.h"
#include <QPainter>
Tile::Tile(QGraphicsItem *parent) : QGraphicsItem(parent)
{
}
void Tile::setImage(QImage image,int x,int y)
{
    m_image = image;
    this->setPos(x * 256,y * 256);
}
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(0,0,m_image);
}
QRectF Tile::boundingRect() const
{
    return QRectF(0,0,m_image.width(),m_image.height());
    //return QRectF(-0.5,-0.5,1,1);
}
