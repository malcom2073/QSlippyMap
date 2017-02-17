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

#ifndef JSONDATASOURCE_H
#define JSONDATASOURCE_H
#include "datasource.h"
#include <QSerialPort>
#include <QFile>

class JSONDataSource : public DataSource
{
	Q_OBJECT
public:
	JSONDataSource();
	void start();
	void loadFile(QString filename);
	void setConnectionSpecific(QString key, QVariant value);
private:
	QString m_portName;
	int m_baudRate;
	QSerialPort *m_port;
	QByteArray buffer;
	void parseBuffer();
	QFile *m_logFile;
	quint64 m_lastMsecs;
private slots:
	void readyRead();
};

#endif // JSONDATASOURCE_H
