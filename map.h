#ifndef MAP_H
#define MAP_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "tile.h"
#include "tilecache.h"
class Map : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Map(QWidget *parent = 0);
    void addWaypoint(double lat,double lon,double accel);
    void setCurrentPosition(double lat, double lon);
    void centerOn(double lat,double lon);
    void setCenter(double lat, double lon,int zoom);
    int getZoom() { return m_zoomLevel; }
    void setZoom(int zoom);
private:
    QList<QPair<double,double> >m_waypoints;
    TileCache *m_tileCache;
    int m_zoomLevel;
    QGraphicsScene *m_scene;
    QMap<QNetworkReply*,QPair<QPair<int,int>,int > > m_tileIdList;
    void mouseMoveEvent(QMouseEvent *evt);
    void mousePressEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);
    void wheelEvent(QWheelEvent *evt);
    QPointF mapToSceneCoords(QPointF latlon);
    QPointF sceneToMapCoords(QPointF scenecoords);
    QGraphicsEllipseItem *m_cursorCircle;
    QPointF m_lastLinePoint;
    QGraphicsEllipseItem *m_currentPosition;
    bool m_mouseIsDown;
    QPointF m_lastMousePos;
    QPointF m_currentLatLon;
    QString m_cacheLocation;
signals:
    void mouseMoved(double lat, double lon);
    void mouseReleased(double lat, double lon);
    void tileUpdates(int waiting);
    void localTileUpdate(int count);
    void networkTileUpdate(int count);

private slots:
    void tileRecv(int x,int y, int z, QImage tile);

public slots:

};

#endif // MAP_H
