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

#ifndef TILECACHE_H
#define TILECACHE_H
#include <QMap>
#include <QImage>
#include <QThread>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
class TileCache : public QThread
{
	Q_OBJECT
public:
	TileCache();
	void add(int x, int y, int z, QImage img);
	void getTile(int x, int y, int z);
	bool contains(int x, int y, int z);
	void zoomLevelChanged();
private:
	int Random(int low, int high);
	class TileReq
	{
	public:
		int x;
		int y;
		int z;
	};
	QList<TileReq> m_tileQueue;
	QMutex m_tileMutex;
	QMutex m_networkQueueMutex;
	QMap<int,QMap<int,QMap<int,bool> > > m_tileExistsMap;
	QString m_cacheLocation;
	void run();
	QNetworkAccessManager *m_nam;
	QMap<QNetworkReply*,QPair<QPair<int, int>, int> > m_tileIdList;
	QByteArray UserAgent;
signals:
	void tileRecv(int x,int y, int z, QImage tile);
	void localTileUpdate(int count);
	void networkTileUpdate(int count);
private slots:
	void networkFinished();
};

#endif // TILECACHE_H
