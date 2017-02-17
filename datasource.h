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
