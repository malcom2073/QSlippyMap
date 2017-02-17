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

#ifndef NMEADATASOURCE_H
#define NMEADATASOURCE_H
#include "datasource.h"
#include <QtSerialPort/QSerialPort>
#include <QFile>
class NMEADataSource : public DataSource
{
	Q_OBJECT
public:
	NMEADataSource();
	void loadFile(QString filename){}
	void start();
private:
	QSerialPort *m_serialPort;
	QByteArray m_buffer;
	void parseBuffer();
	double m_savedSpeed;
	QFile *m_logFile;

	QFile *m_nmeaLogFile;
private slots:
	void serialReadyRead();
};

#endif // NMEADATASOURCE_H
