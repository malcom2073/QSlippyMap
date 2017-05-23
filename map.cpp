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
	setCenter(lat,lon,18);
	m_mouseIsDown = false;

	this->setTransformationAnchor(QGraphicsView::NoAnchor);
	//this->setResizeAnchor(QGraphicsView::AnchorViewCenter);

	this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

	setMouseTracking(true);
}
void Map::setCenter(double lat, double lon,int zoom)
{
	if (zoom != m_zoomLevel)
	{
		m_tileCache->zoomLevelChanged();
	}
	m_zoomLevel = zoom;
	m_currentLatLon = QPointF(lon,lat);
	m_scene->clear();
	m_currentPosition = 0;

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
    int currtilex = long2tilex(lat,m_zoomLevel);

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
					qDebug() << "Removing item";
					m_scene->removeItem(items.at(0));
					delete items.at(0);
				}
				else
				{
					qDebug() << "No items to remove";
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
					qDebug() << "Removing item";
					m_scene->removeItem(items.at(0));
					delete items.at(0);
				}
				else
				{
					qDebug() << "No items to remove";
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
					qDebug() << "Removing item";
					m_scene->removeItem(items.at(0));
					delete items.at(0);
				}
				else
				{
					qDebug() << "No items to remove";
				}
				m_tileCache->getTile(m_currentTileCoords.x()+x,m_currentTileCoords.y()-5,m_zoomLevel);
			}

		}
		else if (tiley > m_currentTileCoords.y())
		{
			for (int x=-5;x<5;x++)
			{
				QList<QGraphicsItem*> items = m_scene->items(QPointF((m_currentTileCoords.x()-x)*256,(m_currentTileCoords.y()+5)*256));
				if (items.count() > 0)
				{
					qDebug() << "Removing item";
					m_scene->removeItem(items.at(0));
					delete items.at(0);
				}
				else
				{
					qDebug() << "No items to remove";
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
	qDebug() << evt->x() << evt->y();
	QPointF latlon = sceneToMapCoords(mapToScene(evt->x(),evt->y()));
	if (evt->delta() > 0)
	{
		setCenter(latlon.y(),latlon.x(),m_zoomLevel+1);
	}
	else
	{
		setCenter(latlon.y(),latlon.x(),m_zoomLevel-1);
	}
}
void Map::tileRecv(int x,int y, int z, QImage tile)
{
	if (z != m_zoomLevel)
	{
		return;
	}
	Tile *t = new Tile();

	t->setFlag(QGraphicsItem::ItemIsMovable,false);
	t->setFlag(QGraphicsItem::ItemIsSelectable,false);
	t->setImage(tile,x,y);
	m_scene->addItem(t);
	m_scene->setSceneRect(QRectF());
	m_scene->update();
	//this->update();

	//qDebug() << "Adding tile at:" << x*256 << y*256;
	//qDebug() << "New scene:" << m_scene->sceneRect();
}
void Map::setZoom(int zoom)
{
	setCenter(m_currentLatLon.y(),m_currentLatLon.x(),zoom);
}
