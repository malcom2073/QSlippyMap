#ifndef GPSDDATASOURCE_H
#define GPSDDATASOURCE_H
#include "datasource.h"
#include "gpsd.h"
class GPSDDataSource : public DataSource
{
	Q_OBJECT
public:
	GPSDDataSource();
	void start();
private:
	void loadFile(QString filename) {}
	GPSd *m_gpsd;
private slots:
	void tpvSignal(QString type,QString devices, int mode, double lat, double lon, double alt,double speed);
};

#endif // GPSDDATASOURCE_H
