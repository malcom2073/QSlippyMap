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

#ifndef GPSD_H
#define GPSD_H

#include <QObject>
#include <QTcpSocket>
class GPSd : public QObject
{
	Q_OBJECT
public:
	explicit GPSd(QObject *parent = 0);
	void connectToDaemon(QString host, int port = 2947);
private:
	QTcpSocket *m_gpsdSocket;
	void parseBuffer();
	QByteArray m_buffer;
signals:
	void tpvSignal(QString type,QString devices, int mode, double lat, double lon, double alt,double speed);
private slots:
	void connected();
	void readyRead();
public slots:

};

#endif // GPSD_H
