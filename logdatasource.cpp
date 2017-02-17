#include "logdatasource.h"

LogDataSource::LogDataSource() : DataSource()
{
	m_timer = new QTimer(this);
	connect(m_timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));

}
void LogDataSource::start()
{
	m_timer->start(70);
}

DataClass LogDataSource::parseLine(QString line)
{
	DataClass data;
	QStringList split = line.split(",");
	if (split.size() < 5)
	{
		return data;
	}
	QString latstr = split[5];
	QString lonstr = split[6];
	QString speedstr = split[11];
	data.latitude = latstr.toDouble();
	data.longitutde = lonstr.toDouble();;
	data.speed = speedstr.toDouble();
	return data;
}
void LogDataSource::timerTimeout()
{
	if (linelist.size() == 0)
	{
		m_timer->stop();
		return;
	}
	QString line = linelist.at(0);
	linelist.removeAt(0);
	DataClass data = parseLine(line);
	emit incomingData(data);
}
void LogDataSource::loadFile(QString filename)
{
	QFile m_logFile(filename);
	m_logFile.setFileName(filename);
	m_logFile.open(QIODevice::ReadOnly);
	QString filestr = m_logFile.readAll();
	linelist = filestr.split("\n");
	m_logFile.close();
}
