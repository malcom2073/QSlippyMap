#ifndef LOGDATASOURCE_H
#define LOGDATASOURCE_H
#include "datasource.h"
#include <QTimer>
#include <QFile>
#include <QStringList>
class LogDataSource : public DataSource
{
	Q_OBJECT
public:
	LogDataSource();
	void loadFile(QString filename);
	void start();
private:
	DataClass parseLine(QString packet);
	QTimer *m_timer;
	QStringList linelist;
private slots:
	void timerTimeout();
};

#endif // LOGDATASOURCE_H
