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
