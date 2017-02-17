/************************************************************************************
 * QSlippyMap - Tile based slippery map                                             *
 * Copyright (C) 2017  Michael Carpenter (malcom2073@gmail.com)                     *
 *                                                                                  *
 * This file is a part of QSlippyMap                                                *
 *                                                                                  *
 * QSlippyMap is free software; you can redistribute it and/or                      *
 * modify it under the terms of the GNU Lesser General Public                       *
 * License as published by the Free Software Foundation, version                    *
 * 2.1 of the License.                                                              *
 *                                                                                  *
 * QSlippyMap is distributed in the hope that it will be useful,                    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                *
 * Lesser General Public License for more details.                                  *
 *                                                                                  *
 * You should have received a copy of the GNU Lesser General Public                 *
 * License along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   *
 ************************************************************************************/

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
