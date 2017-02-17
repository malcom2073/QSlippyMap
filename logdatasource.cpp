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

#include "logdatasource.h"

LogDataSource::LogDataSource() : DataSource()
{
	m_timer = new QTimer(this);
	connect(m_timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));

}
void LogDataSource::start()
{
	m_timer->start(70);
}

DataClass LogDataSource::parseLine(QString line)
{
	DataClass data;
	QStringList split = line.split(",");
	if (split.size() < 5)
	{
		return data;
	}
	QString latstr = split[5];
	QString lonstr = split[6];
	QString speedstr = split[11];
	data.latitude = latstr.toDouble();
	data.longitutde = lonstr.toDouble();;
	data.speed = speedstr.toDouble();
	return data;
}
void LogDataSource::timerTimeout()
{
	if (linelist.size() == 0)
	{
		m_timer->stop();
		return;
	}
	QString line = linelist.at(0);
	linelist.removeAt(0);
	DataClass data = parseLine(line);
	emit incomingData(data);
}
void LogDataSource::loadFile(QString filename)
{
	QFile m_logFile(filename);
	m_logFile.setFileName(filename);
	m_logFile.open(QIODevice::ReadOnly);
	QString filestr = m_logFile.readAll();
	linelist = filestr.split("\n");
	m_logFile.close();
}
