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

#include "gpsd.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonValue>
GPSd::GPSd(QObject *parent) :
	QObject(parent)
{
}
void GPSd::connectToDaemon(QString host, int port)
{
	m_gpsdSocket = new QTcpSocket(this);
	connect(m_gpsdSocket,SIGNAL(connected()),this,SLOT(connected()));
	connect(m_gpsdSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
	m_gpsdSocket->connectToHost(host,port);
}
void GPSd::connected()
{
	m_gpsdSocket->write("?WATCH={\"enable\":true,\"json\":true}");
}
void GPSd::readyRead()
{
	m_buffer.append(m_gpsdSocket->readAll());
	parseBuffer();
}
void GPSd::parseBuffer()
{
	if (!m_buffer.contains("\n"))
	{
		return;
	}
	QByteArray line = m_buffer.mid(0,m_buffer.indexOf("\n")+1);
	m_buffer = m_buffer.mid(line.length());
	qDebug() << line;
	QJsonDocument doc = QJsonDocument::fromJson(line);
	if (doc.isEmpty())
	{
		qDebug() << "Error";
	}
	QJsonObject topobject = doc.object();
	if (topobject.value("class").toString() == "VERSION")
	{
	}
	else if (topobject.value("class").toString() == "DEVICES")
	{
	}
	else if (topobject.value("class").toString() == "WATCH")
	{
	}
	else if (topobject.value("class").toString() == "TPV")
	{
		//void tpvSignal(QString type,QString devices, int mode, double lat, double lon, double alt);
		emit tpvSignal(topobject.value("tag").toString(),topobject.value("device").toString(),topobject.value("mode").toInt(),topobject.value("lat").toDouble(),topobject.value("lon").toDouble(),topobject.value("alt").toDouble(),topobject.value("speed").toDouble());
	}



	parseBuffer();
}
