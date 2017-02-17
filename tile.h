#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>

class Tile : public QGraphicsItem
{
public:
    explicit Tile(QGraphicsItem *parent = 0);
    void setImage(QImage image,int x,int y);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
    QRectF boundingRect() const;
private:
    QImage m_image;
signals:

public slots:

};

#endif // TILE_H
