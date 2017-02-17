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

#include "jsondatasource.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>
JSONDataSource::JSONDataSource()
{
}
void JSONDataSource::start()
{
	m_port = new QSerialPort(this);
	connect(m_port,SIGNAL(readyRead()),this,SLOT(readyRead()));
	m_port->setPortName(m_portName);
	m_port->open(QIODevice::ReadWrite);
	m_port->setBaudRate(57600);
	m_port->setParity(QSerialPort::NoParity);
	m_port->setStopBits(QSerialPort::OneStop);
	m_port->setFlowControl(QSerialPort::NoFlowControl);
	m_port->setDataBits(QSerialPort::Data8);
	m_logFile = new QFile(QString::number(QDateTime::currentMSecsSinceEpoch(),'f',0) + ".txt");
	m_logFile->open(QIODevice::ReadWrite);

}

void JSONDataSource::loadFile(QString filename)
{

}
void JSONDataSource::readyRead()
{
	QByteArray buf = m_port->readAll();
	m_logFile->write(buf);
	m_logFile->flush();
	buffer.append(buf);
	parseBuffer();
}
void JSONDataSource::parseBuffer()
{
	QByteArray line;
	if (buffer.contains("\n"))
	{
		line = buffer.mid(0,buffer.indexOf("\n"));
		buffer = buffer.mid(buffer.indexOf("\n")+1);
		qDebug() << line;
	}
	else
	{
		return;
	}
	QJsonDocument doc = QJsonDocument::fromJson(line);
	QJsonObject topobject = doc.object();
	if (topobject.contains("type"))
	{
		if (topobject.value("type").toString() == "navpos")
		{
			m_lastMsecs = topobject.value("msec").toInt();
			DataClass data;
			data.msecs = m_lastMsecs;
			data.latitude = topobject.value("lat").toDouble();
			data.longitutde = topobject.value("lon").toDouble();
			data.imuvalid = false;
			data.locationvalid = true;
			emit incomingData(data);
		}
		else if (topobject.value("type").toString() == "imu")
		{
			if (m_lastMsecs == 0)
			{
				//Don't parse 0
				return;
			}
			DataClass data;
			data.msecs = m_lastMsecs;
			data.imuvalid = true;
			data.locationvalid = false;
			data.gyrox = topobject.value("gyrox").toDouble();
			data.gyroy = topobject.value("gyroy").toDouble();
			data.gyroz = topobject.value("gyroz").toDouble();
			data.accelx = topobject.value("accelx").toDouble();
			data.accely = topobject.value("accely").toDouble();
			data.accelz = topobject.value("accelz").toDouble();
			emit incomingData(data);

		}
	}

	parseBuffer();
}
void JSONDataSource::setConnectionSpecific(QString key, QVariant value)
{
	if (key == "portname")
	{
		m_portName = "\\\\.\\" + value.toString();
	}
	else if (key == "baud")
	{
		m_baudRate = value.toInt();
	}
}
