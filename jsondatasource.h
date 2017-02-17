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
