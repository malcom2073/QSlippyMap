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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
class DataClass
{
public:
	quint64 msecs;
	double latitude;
	double longitutde;
	double speed;
	double heading;
	double gyrox;
	double gyroy;
	double gyroz;
	double accelx;
	double accely;
	double accelz;
	bool imuvalid;
	bool locationvalid;
};

class DataSource : public QObject
{
	Q_OBJECT
public:
	DataSource();
	virtual void start() = 0;
	virtual void loadFile(QString filename)=0;
	virtual void setConnectionSpecific(QString key, QVariant value)=0;
signals:
	void incomingData(DataClass data);
};

#endif // DATASOURCE_H
