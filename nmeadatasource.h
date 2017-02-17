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
