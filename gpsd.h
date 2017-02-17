#ifndef GPSD_H
#define GPSD_H

#include <QObject>
#include <QTcpSocket>
class GPSd : public QObject
{
	Q_OBJECT
public:
	explicit GPSd(QObject *parent = 0);
	void connectToDaemon(QString host, int port = 2947);
private:
	QTcpSocket *m_gpsdSocket;
	void parseBuffer();
	QByteArray m_buffer;
signals:
	void tpvSignal(QString type,QString devices, int mode, double lat, double lon, double alt,double speed);
private slots:
	void connected();
	void readyRead();
public slots:

};

#endif // GPSD_H
