#include "mainwindow.h"
#include <QApplication>
#include <QStringList>
int main(int argc, char *argv[])
{
	QString first = "0100";
	QString second = "100";
	double firstd = first.toDouble();
	double secondd = second.toDouble();

	QString line = "$GPGGA,193714.395,3909.3798,N,07632.0972,W,1,10,0.9,35.1,M,-35.0,M,,0000*56";
	QStringList linesplit = line.split(",");

	double londeg = linesplit[4].split(".")[0].mid(0,3).toDouble();
	double lonmin = linesplit[4].split(".")[0].mid(3,2).toDouble();
	double londec = linesplit[4].split(".")[1].toDouble();
	QString lonnum = QString::number(lonmin) + "." + QString::number(londec);
	double lonfin = lonnum.toDouble();
	double lon = londeg + (lonfin * (1.0/60.0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
