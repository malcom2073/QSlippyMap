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

#include "gpsddatasource.h"

GPSDDataSource::GPSDDataSource()
{
	m_gpsd = new GPSd();
	connect(m_gpsd,SIGNAL(tpvSignal(QString,QString,int,double,double,double,double)),this,SLOT(tpvSignal(QString,QString,int,double,double,double,double)));
	m_gpsd->connectToDaemon("192.168.1.122");
}
void GPSDDataSource::start()
{

}
void GPSDDataSource::tpvSignal(QString type,QString devices, int mode, double lat, double lon, double alt,double speed)
{
	if (type == "GGA")
	{
		DataClass data;
		data.latitude = lat;
		data.longitutde = lon;
		data.speed = speed;
		emit incomingData(data);
	}
}
