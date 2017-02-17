#include "nmeadatasource.h"
#include <QDebug>
#include <QDateTime>
NMEADataSource::NMEADataSource()
{
}
void NMEADataSource::start()
{
	m_logFile = new QFile(QString::number(QDateTime::currentMSecsSinceEpoch(),'f',0) + ".txt");
	m_logFile->open(QIODevice::ReadWrite);

	m_nmeaLogFile = new QFile(QString::number(QDateTime::currentMSecsSinceEpoch(),'f',0) + "NMEA.txt");
	m_nmeaLogFile->open(QIODevice::ReadWrite);


	m_serialPort = new QSerialPort(this);
	m_serialPort->setPortName("COM6");

	m_serialPort->open(QIODevice::ReadOnly);
	m_serialPort->setBaudRate(9600);
	connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(serialReadyRead()));
}
void NMEADataSource::serialReadyRead()
{
	m_buffer.append(m_serialPort->readAll());
	parseBuffer();
}
void NMEADataSource::parseBuffer()
{
	if (m_buffer.contains("\n"))
	{
		QString line = QString(m_buffer.mid(0,m_buffer.indexOf("\n")+1));
		m_nmeaLogFile->write(QString(line + "\r\n").toLatin1());
		m_nmeaLogFile->flush();
		m_buffer.remove(0,line.length());
		if (line.startsWith("$GPVTG"))
		{
			QStringList linesplit = line.split(",");
			if (linesplit.size() > 7)
			{
				double speed = linesplit[7].toDouble();
				m_savedSpeed = speed;
			}
		}
		if (line.startsWith("$GPGGA"))
		{
			//NMEA DATER!
			qDebug() << "Dater:" << line;
			QStringList linesplit = line.split(",");
			if (linesplit.size() > 9)
			{
				double latdeg = linesplit[2].split(".")[0].mid(0,2).toDouble();
				double latmin = linesplit[2].mid(2).toDouble();
				double lat = latdeg + (latmin / 60.0);

				double londeg = linesplit[4].split(".")[0].mid(0,3).toDouble();
				double lonmin = linesplit[4].mid(3).toDouble();
				double lon = londeg + (lonmin / 60.0);


				//Lon/Lat is in the opposite notation, not normal N/E
				if (linesplit[3] == "S")
				{
					lat = 0-lat;
				}
				if (linesplit[5] == "W")
				{
					lon = 0-lon;
				}
				long time = linesplit[1].toDouble();
				int sats = linesplit[7].toInt();
				double alt = linesplit[9].toDouble();
				//location(lon,lat,time,fix,sats,alt);
				DataClass data;
				data.latitude = lat;
				data.longitutde = lon;
				data.speed = m_savedSpeed;
				emit incomingData(data);
				m_logFile->write(QString("0,0,0,0,0," + QString::number(data.latitude,'f',10) +"," + QString::number(data.longitutde,'f',10) + ",0,0,0,0," + QString::number(data.speed,'f',4) + "\r\n").toLatin1());
				m_logFile->flush();
			}
		}
	}
	if (m_buffer.contains("\n"))
	{
		parseBuffer();
	}
}
