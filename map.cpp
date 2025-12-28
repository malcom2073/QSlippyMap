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

#include "map.h"
#define M_PI       3.14159265358979323846
#include "functions.h"
#include <QMouseEvent>
#include <QPen>
#include <QStandardPaths>
#include <QDir>
#include <QtMath>
#include <QDebug>

Map::Map(QWidget *parent) : QGraphicsView(parent)
{
	m_currentPosition=0;
	m_scene = new QGraphicsScene(this);
	this->setScene(m_scene);

	//setDragMode(ScrollHandDrag);

	m_tileCache = new TileCache();
	connect(m_tileCache,SIGNAL(tileRecv(int,int,int,QImage)),this,SLOT(tileRecv(int,int,int,QImage)));
	connect(m_tileCache,SIGNAL(localTileUpdate(int)),this,SIGNAL(localTileUpdate(int)));
	connect(m_tileCache,SIGNAL(networkTileUpdate(int)),this,SIGNAL(networkTileUpdate(int)));
	//setAlignment(Qt::NoAlignment);

	//double lat =  39.359194;
	//double lon = -75.069349;
	//double lat = 36.561288;
	//double lon = -79.207576;
	double lat = 39.155955;
	double lon = -76.535755;
	m_targetZoom = 18;
	m_displayedZoomLevel = 18;
	setCenter(lat,lon,18);
	m_mouseIsDown = false;

	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	//this->setResizeAnchor(QGraphicsView::AnchorViewCenter);

	this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

	setMouseTracking(true);
}
void Map::SetGoogle()
{
    m_tileCache->SetGoogle();
    int tilex = m_currentTileCoords.x();
    int tiley = m_currentTileCoords.y();
    for (int x=-5;x<5;x++)
    {
        for (int y=-5;y<5;y++)
        {
            m_tileCache->getTile(tilex+x,tiley+y,m_zoomLevel);
        }
    }
}
void Map::SetMapBox()
{
    m_tileCache->SetMapbox();
    int tilex = m_currentTileCoords.x();
    int tiley = m_currentTileCoords.y();
    for (int x=-5;x<5;x++)
    {
        for (int y=-5;y<5;y++)
        {
            m_tileCache->getTile(tilex+x,tiley+y,m_zoomLevel);
        }
    }
}
void Map::setCenter(double lat, double lon,int zoom)
{
	bool zoomChanged = (zoom != m_zoomLevel);
	
	if (zoomChanged)
	{
		m_tileCache->zoomLevelChanged();
		// Update target zoom to match
		m_targetZoom = zoom;
		m_displayedZoomLevel = zoom;
	}
	
	m_zoomLevel = zoom;
	m_currentLatLon = QPointF(lon,lat);
	
	if (m_scene->items().count() < 10 || !zoomChanged)
	{
		m_scene->clear();
		m_currentPosition = 0;
	}

	int tilex = long2tilex(lon,m_zoomLevel);
	int tiley = lat2tiley(lat,m_zoomLevel);

	m_scene->setSceneRect((tilex-5) * 256,(tiley-5)*256,10*256,10*256);
	//QPointF whmap = mapToScene(width(),height());
	//this->setSceneRect((tilex-5) * 256,(tiley-5)*256,10*256,10*256);
	this->centerOn(lat,lon);
	qDebug() << m_scene->sceneRect();
	qDebug() << this->sceneRect();
	//m_scene->setSceneRect((tilex-5) * 256,(tiley-5)*256,whmap.x(),whmap.y());
	//m_scene->setSceneRect();
	m_currentTileCoords.setX(tilex);
	m_currentTileCoords.setY(tiley);



    for (int x=-5;x<5;x++)
    {
        for (int y=-5;y<5;y++)
        {
            m_tileCache->getTile(tilex+x,tiley+y,m_zoomLevel);
            /*if (m_tileCache->contains(tilex+x,tiley+y,m_zoomLevel))
            {
                m_tileCache->getTile(tilex+x,tiley+y,m_zoomLevel);

            }
            else
            {
                QNetworkRequest req;
                QString url = "https://mt0.google.com/vt/";
                QString loc = "lyrs=s&x=%1&y=%2&z=%3";
                req.setUrl(url + loc.arg(tilex+x).arg(tiley+y).arg(m_zoomLevel));

                QNetworkReply *reply = m_nam->get(req);
                m_tileIdList[reply] = QPair<QPair<int,int> , int>(QPair<int,int>(tilex+x,tiley+y),m_zoomLevel);
                connect(reply,SIGNAL(finished()),this,SLOT(networkFinished()));
                emit tileUpdates(m_tileIdList.keys().count());
            }*/
        }
    }
	QGraphicsLineItem *item = new QGraphicsLineItem();
	m_scene->addItem(item);
	m_cursorCircle = new QGraphicsEllipseItem();
	m_scene->addItem(m_cursorCircle);
	m_cursorCircle->setZValue(1);
	QPen pen = m_cursorCircle->pen();
	pen.setWidth(5);
	m_cursorCircle->setPen(pen);

	for (int i=0;i<m_waypoints.size();i++)
	{
		QPointF scenecoords = mapToSceneCoords(QPointF(m_waypoints.at(i).first,m_waypoints.at(i).second));
		if (i == 0)
		{
			m_lastLinePoint = scenecoords;
		}

		QGraphicsEllipseItem *wpitem = new QGraphicsEllipseItem(scenecoords.x()-2,scenecoords.y()-2,4,4);
		QPen pen = wpitem->pen();
		pen.setWidth(2);
		wpitem->setPen(pen);
		m_scene->addItem(wpitem);
		wpitem->setZValue(2);

		QGraphicsLineItem *wpline = new QGraphicsLineItem(m_lastLinePoint.x(),m_lastLinePoint.y(),scenecoords.x(),scenecoords.y());
		pen.setColor(QColor::fromRgb(0,0,0));
		wpline->setPen(pen);
		m_scene->addItem(wpline);
		wpline->setZValue(1);
		m_lastLinePoint.setX(scenecoords.x());
		m_lastLinePoint.setY(scenecoords.y());
	}
}

void Map::setCurrentPosition(double lat, double lon)
{
    QPointF scenecoords = mapToSceneCoords(QPointF(lon,lat));
    if (!m_currentPosition)
    {
            m_currentPosition = new QGraphicsEllipseItem(scenecoords.x()-3,scenecoords.y()-3,6,6);
            QPen pen = m_currentPosition->pen();
            pen.setColor(QColor::fromRgb(0,255,0));
            pen.setWidth(2);
            m_currentPosition->setPen(pen);
	    m_currentPosition->setZValue(3);
            m_scene->addItem(m_currentPosition);
            return;
    }
    m_currentPosition->setRect(scenecoords.x()-3,scenecoords.y()-3,6,6);
}
void Map::centerOn(double lat,double lon)
{
    QPointF scenecoords = mapToSceneCoords(QPointF(lon,lat));
    QGraphicsView::centerOn(scenecoords);
}

void Map::addWaypoint(double lat,double lon,double accel)
{

    QPointF scenecoords = mapToSceneCoords(QPointF(lon,lat));
    QGraphicsEllipseItem *wpitem = new QGraphicsEllipseItem(scenecoords.x()-2,scenecoords.y()-2,4,4);
    QPen pen = wpitem->pen();
    if (accel > 0)
    {
        pen.setColor(QColor::fromRgb(0,((accel) / 0.15) * 255 ,0));
    }
    else
    {
        pen.setColor(QColor::fromRgb(((accel + 0.20) / 0.15) * 255 ,0,0));
    }
    pen.setWidth(2);
    wpitem->setPen(pen);
    m_scene->addItem(wpitem);
    wpitem->setZValue(2);
    QGraphicsLineItem *wpline = new QGraphicsLineItem(m_lastLinePoint.x(),m_lastLinePoint.y(),scenecoords.x(),scenecoords.y());
    m_waypoints.append(QPair<double,double>(lon,lat));
    pen.setColor(QColor::fromRgb(0,0,0));
    wpline->setPen(pen);
    m_scene->addItem(wpline);
    wpline->setZValue(1);
    m_lastLinePoint.setX(scenecoords.x());
    m_lastLinePoint.setY(scenecoords.y());
}

QPointF Map::sceneToMapCoords(QPointF scenecoords)
{
    //Convert scene coords to lat/lon

    double currtilex = ((int)(scenecoords.x()/256)) * 256;
    double nexttilex = ((int)(scenecoords.x()/256)+1) * 256;

    double currtiley = ((int)(scenecoords.y()/256)) * 256;
    double nexttiley = ((int)(scenecoords.y()/256)+1) * 256;


    double lat = tiley2lat(scenecoords.y()/256,m_zoomLevel);
    double lon = tilex2long(scenecoords.x()/256,m_zoomLevel);
    double latnext = tiley2lat((scenecoords.y()/256)+1,m_zoomLevel);
    double lonnext = tilex2long((scenecoords.x()/256)+1,m_zoomLevel);


    double newlat = lat + ((latnext - lat) * ((scenecoords.y() - currtiley) / (nexttiley - currtiley)));
    double newlon = lon + ((lonnext - lon) * ((scenecoords.x() - currtilex) / (nexttilex - currtilex)));
    return QPointF(newlon,newlat);
}
QPointF Map::mapToSceneCoords(QPointF latlon)
{
    //Convert lat/lon to scene coords
    double lat = latlon.y();
    double lon = latlon.x();
    int currtiley = lat2tiley(lat,m_zoomLevel);
    int currtilex = long2tilex(lon,m_zoomLevel);  // Fixed: use lon, not lat

    double currlat = tiley2lat(currtiley,m_zoomLevel);
    double currlon = tilex2long(currtilex,m_zoomLevel);

    double nextlat = tiley2lat(currtiley+1,m_zoomLevel);
    double nextlon = tilex2long(currtilex+1,m_zoomLevel);

    double sceney = (currtiley * 256) + (256 * ((lat-currlat) / (nextlat - currlat)));
    double scenex = (currtilex * 256) + (256 * ((lon-currlon) / (nextlon - currlon)));

    return QPointF(scenex,sceney);

}

void Map::mouseMoveEvent(QMouseEvent *evt)
{
	QPointF scenecoords = mapToScene(evt->x(),evt->y());
	QPointF realcentercoords = mapToScene(width()/2,height()/2);
	QPointF realcenterlatlon = sceneToMapCoords(realcentercoords);
	QPointF latlon = sceneToMapCoords(scenecoords);

	emit mouseMoved(latlon.y(),latlon.x());

	QPointF newscene = mapToSceneCoords(latlon);
	m_cursorCircle->setRect(newscene.x()-10,newscene.y()-10,20,20);
	if (m_mouseIsDown)
	{
		//Recalculate which tiles should be shown, and possibly remove old ones?
		/*int tilex = long2tilex(latlon.x(),m_zoomLevel);
		int tiley = lat2tiley(latlon.y(),m_zoomLevel);
		for (int x=-5;x<5;x++)
		{
			for (int y=-5;y<5;y++)
			{
				m_tileCache->getTile(tilex+x,tiley+y,m_zoomLevel);
			}
		}*/
		int tilex = long2tilex(realcenterlatlon.x(),m_zoomLevel);
		int tiley = lat2tiley(realcenterlatlon.y(),m_zoomLevel);
		if (tilex < m_currentTileCoords.x())
		{
			//Center has shifted left/right, remove one layer of images and add another.
			for (int y=-5;y<5;y++)
			{
				QList<QGraphicsItem*> items = m_scene->items(QPointF((m_currentTileCoords.x()+5)*256,(m_currentTileCoords.y()+y)*256));
				if (items.count() > 0)
				{
					m_scene->removeItem(items.at(0));
					delete items.at(0);
				}
				m_tileCache->getTile(m_currentTileCoords.x()-5,m_currentTileCoords.y()+y,m_zoomLevel);
			}
		}
		else if (tilex > m_currentTileCoords.x())
		{
			for (int y=-5;y<5;y++)
			{
				QList<QGraphicsItem*> items = m_scene->items(QPointF((m_currentTileCoords.x()-5)*256,(m_currentTileCoords.y()+y)*256));
				if (items.count() > 0)
				{
					m_scene->removeItem(items.at(0));
					delete items.at(0);
				}
				m_tileCache->getTile(m_currentTileCoords.x()+5,m_currentTileCoords.y()+y,m_zoomLevel);
			}

		}
		if (tiley < m_currentTileCoords.y())
		{
			//Center has shifted left/right, remove one layer of images and add another.
			for (int x=-5;x<5;x++)
			{
				QList<QGraphicsItem*> items = m_scene->items(QPointF((m_currentTileCoords.x()+x)*256,(m_currentTileCoords.y()+5)*256));
				if (items.count() > 0)
				{
					m_scene->removeItem(items.at(0));
					delete items.at(0);
				}
				m_tileCache->getTile(m_currentTileCoords.x()+x,m_currentTileCoords.y()-5,m_zoomLevel);
			}

		}
		else if (tiley > m_currentTileCoords.y())
		{
			for (int x=-5;x<5;x++)
			{
				QList<QGraphicsItem*> items = m_scene->items(QPointF((m_currentTileCoords.x()+x)*256,(m_currentTileCoords.y()-5)*256));
				if (items.count() > 0)
				{
					m_scene->removeItem(items.at(0));
					delete items.at(0);
				}
				m_tileCache->getTile(m_currentTileCoords.x()+x,m_currentTileCoords.y()+5,m_zoomLevel);
			}
		}
		m_currentTileCoords.setX(tilex);
		m_currentTileCoords.setY(tiley);
		//qDebug() << "Center tile:" << tilex << tiley;
		this->translate(scenecoords.x() - m_lastMousePos.x(),scenecoords.y() - m_lastMousePos.y());
		//qreal scenex = sceneRect().x() + (scenecoords.x() - m_lastMousePos.x());
		//qreal sceney = sceneRect().y() + (scenecoords.y() - m_lastMousePos.y());
		//this->setSceneRect(scenex,sceney,sceneRect().width(),sceneRect().height());
		//qDebug() << scenex << sceney;
		//qDebug() << scenecoords.x() - m_lastMousePos.x() << scenecoords.y() - m_lastMousePos.y();
		//qDebug() << this->transform();
		m_lastMousePos = mapToScene(evt->x(),evt->y());
		update();
		//qDebug() << QString::number(sceneRect().x(),'f') << QString::number(m_scene->sceneRect().x(),'f');
	}
	QGraphicsView::mouseMoveEvent(evt);
}
void Map::mousePressEvent(QMouseEvent *evt)
{
	m_mouseIsDown = true;
	m_lastMousePos = mapToScene(evt->x(),evt->y());
	// setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	QGraphicsView::mousePressEvent(evt);

}
void Map::mouseReleaseEvent(QMouseEvent *evt)
{
	QPointF scenecoords = this->mapToScene(evt->x(),evt->y());
	QPointF latlon = sceneToMapCoords(scenecoords);

	emit mouseReleased(latlon.y(),latlon.x());
	m_mouseIsDown = false;
	//setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	QGraphicsView::mouseReleaseEvent(evt);
}
void Map::wheelEvent(QWheelEvent *evt)
{
	double zoomDelta = evt->angleDelta().y() / 120.0 * 0.2;  // Each notch is 120, scale by 0.2 for smoothness
	
	m_targetZoom += zoomDelta;
	
	if (m_targetZoom < 1) 
	{
		m_targetZoom = 1;
	}
	if (m_targetZoom > 19)
	{
		m_targetZoom = 19;
	}
	
	double scaleFactor = qPow(2.0, m_targetZoom - m_displayedZoomLevel);
	
	resetTransform();
	scale(scaleFactor, scaleFactor);
	
	checkZoomThreshold();
	
	qDebug() << "Target zoom:" << m_targetZoom << "Display zoom:" << m_displayedZoomLevel << "Scale:" << scaleFactor;
}

void Map::checkZoomThreshold()
{
	int optimalZoom = qRound(m_targetZoom);
	
	if (optimalZoom < 1) optimalZoom = 1;
	if (optimalZoom > 19) optimalZoom = 19;
	
	if (optimalZoom != m_displayedZoomLevel)
	{
		qDebug() << "Switching tile zoom from" << m_displayedZoomLevel << "to" << optimalZoom;
		
		QPointF viewportCenter = viewport()->rect().center();
		QPointF oldSceneCenter = viewportTransform().inverted().map(viewportCenter);
		
		QPointF centerLatLon = sceneToMapCoords(oldSceneCenter);
		
		int oldZoomLevel = m_displayedZoomLevel;
		
		m_tileCache->zoomLevelChanged();
		
		m_displayedZoomLevel = optimalZoom;
		m_zoomLevel = optimalZoom;
		
		m_targetZoom = optimalZoom;
		
		resetTransform();
		
		QPointF newSceneCenter = mapToSceneCoords(centerLatLon);
		printf("Old Scene Center: (%.2f, %.2f)\n", oldSceneCenter.x(), oldSceneCenter.y());
		printf("New Scene Center: (%.2f, %.2f)\n", newSceneCenter.x(), newSceneCenter.y());

		QPointF oldLatLon = sceneToMapCoords(oldSceneCenter);
		QPointF newLatLon = sceneToMapCoords(newSceneCenter);
		printf("Old Center LatLon: (%.6f, %.6f)\n", oldLatLon.y(), oldLatLon.x());
		printf("New Center LatLon: (%.6f, %.6f)\n", newLatLon.y(), newLatLon.x());
		
		int tilex = (int)(newSceneCenter.x() / 256);
		int tiley = (int)(newSceneCenter.y() / 256);
		int oldtilex = m_currentTileCoords.x();
		int oldtiley = m_currentTileCoords.y();
		printf("Old Tile Coords: (%d, %d)\n", oldtilex, oldtiley);
		printf("New Tile Coords: (%d, %d)\n", tilex, tiley);
		
		m_currentTileCoords.setX(tilex);
		m_currentTileCoords.setY(tiley);
		
		m_scene->setSceneRect((tilex-10) * 256, (tiley-10) * 256, 20*256, 20*256);
		
		int zoomDiff = optimalZoom - oldZoomLevel;
		
		for (int x=-7; x<8; x++)
		{
			for (int y=-7; y<8; y++)
			{
				QImage placeholder;
				
				if (zoomDiff == 1)
				{
					int oldTileX = (tilex + x) / 2;
					int oldTileY = (tiley + y) / 2;
					QPointF oldTileCenter(oldTileX * 256 + 128, oldTileY * 256 + 128);
					
					QList<QGraphicsItem*> oldItems = m_scene->items(oldTileCenter);
					Tile *oldTile = nullptr;
					foreach(QGraphicsItem *item, oldItems)
					{
						Tile *t = dynamic_cast<Tile*>(item);
						if (t && t->zValue() == oldZoomLevel)
						{
							oldTile = t;
							break;
						}
					}
					
					if (oldTile)
					{
						int quadX = (tilex + x) % 2;
						int quadY = (tiley + y) % 2;
						
						QImage oldImage = oldTile->getImage();
						if (!oldImage.isNull())
						{
							QImage quadrant = oldImage.copy(quadX * 128, quadY * 128, 128, 128);
							placeholder = quadrant.scaled(256, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
						}
					}
				}
				else if (zoomDiff == -1)
				{
					placeholder = QImage(256, 256, QImage::Format_RGB32);
					placeholder.fill(QColor(230, 230, 230));
					QPainter painter(&placeholder);
					
					for (int qx = 0; qx < 2; qx++)
					{
						for (int qy = 0; qy < 2; qy++)
						{
							int oldTileX = (tilex + x) * 2 + qx;
							int oldTileY = (tiley + y) * 2 + qy;
							QPointF oldTileCenter(oldTileX * 256 + 128, oldTileY * 256 + 128);
							
							QList<QGraphicsItem*> oldItems = m_scene->items(oldTileCenter);
							Tile *oldTile = nullptr;
							foreach(QGraphicsItem *item, oldItems)
							{
								Tile *t = dynamic_cast<Tile*>(item);
								if (t && t->zValue() == oldZoomLevel)
								{
									oldTile = t;
									break;
								}
							}
							
							if (oldTile)
							{
								QImage oldImage = oldTile->getImage();
								if (!oldImage.isNull())
								{
									QImage scaled = oldImage.scaled(128, 128, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
									painter.drawImage(qx * 128, qy * 128, scaled);
								}
							}
						}
					}
				}
				
				if (placeholder.isNull())
				{
					placeholder = QImage(256, 256, QImage::Format_RGB32);
					placeholder.fill(QColor(230, 230, 230));
				}
				
				tileRecv(tilex+x, tiley+y, m_zoomLevel, placeholder);
				
				m_tileCache->getTile(tilex+x, tiley+y, m_zoomLevel);
			}
		}
		
		QGraphicsView::centerOn(newSceneCenter);
		
		cleanupOldTiles(optimalZoom);
	}
}

void Map::tileRecv(int x,int y, int z, QImage tile)
{
	if (z != m_zoomLevel)
	{
		return;
	}
	
	QPointF tileCenter(x * 256 + 128, y * 256 + 128);
	QList<QGraphicsItem*> itemsAtPos = m_scene->items(tileCenter);
	
	foreach(QGraphicsItem *item, itemsAtPos)
	{
		Tile *existingTile = dynamic_cast<Tile*>(item);
		if (existingTile && existingTile->zValue() == z)
		{
			existingTile->setImage(tile, x, y);
			m_scene->update();
			return;
		}
	}
	
	Tile *t = new Tile();
	t->setFlag(QGraphicsItem::ItemIsMovable,false);
	t->setFlag(QGraphicsItem::ItemIsSelectable,false);
	t->setImage(tile,x,y);
	
	t->setZValue(z);
	
	m_scene->addItem(t);
	
	// m_scene->setSceneRect(QRectF());
	m_scene->update();
	
	//qDebug() << "Adding tile at:" << x*256 << y*256 << "zoom:" << z << "Current zoom:" << m_zoomLevel;
}

void Map::cleanupOldTiles(int keepZoomLevel)
{
	QList<QGraphicsItem*> itemsToRemove;
	
	foreach(QGraphicsItem *item, m_scene->items())
	{
		Tile *tile = dynamic_cast<Tile*>(item);
		if (tile)
		{
			int tileZoom = static_cast<int>(tile->zValue());
			
			if (qAbs(tileZoom - keepZoomLevel) > 2)
			{
				itemsToRemove.append(tile);
			}
		}
	}
	
	foreach(QGraphicsItem *item, itemsToRemove)
	{
		m_scene->removeItem(item);
		delete item;
	}
	
	if (itemsToRemove.size() > 0)
	{
		qDebug() << "Cleaned up" << itemsToRemove.size() << "old tiles";
	}
}

void Map::setZoom(int zoom)
{
	setCenter(m_currentLatLon.y(),m_currentLatLon.x(),zoom);
}
