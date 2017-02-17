/************************************************************************************
 * QSlippyMap - Tile based slippery map                                             *
 * Copyright (C) 2017  Michael Carpenter (malcom2073@gmail.com)                     *
 *                                                                                  *
 * This file is a part of QSlippyMap                                                *
 *                                                                                  *
 * QSlippyMap is free software; you can redistribute it and/or                      *
 * modify it under the terms of the GNU Lesser General Public                       *
 * License as published by the Free Software Foundation, version                    *
 * 2.1 of the License.                                                              *
 *                                                                                  *
 * QSlippyMap is distributed in the hope that it will be useful,                    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                *
 * Lesser General Public License for more details.                                  *
 *                                                                                  *
 * You should have received a copy of the GNU Lesser General Public                 *
 * License along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   *
 ************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QLabel>
#include "map.h"
#include "logdatasource.h"
#include "gpsddatasource.h"
#include "qcustomplot.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
	DataSource *m_logDataSource;
	Ui::MainWindow *ui;
	QList<QString> list;
	QTimer *m_timer;
	QTimer *m_lapTimeUpdateTimer;
	double m_centerLat;
	double m_centerLon;
	QPointF m_lastPoint;
	double m_totalAngle;
	double m_lastAngle;
	double m_totalUnsignedAngle;
	double m_calculatedTotalLapAngle;
	double m_calculatedDistance;
	int m_lapCount;
	int state;
	QTime m_startDateTime;
	QTime m_lapStartDateTime;
	QLabel *m_tileUpdateLabel;
	QLabel *m_tileDownloadLabel;
	QCustomPlot *m_gyroPlot;
	QCustomPlot *m_accelPlot;
private slots:
	void mouseMoved(double lat,double lon);
	void mouseClicked(double lat, double lon);
	void replayClicked();
	void incomingData(DataClass data);
	void setStartButtonClicked();
	void lapTimeUpdateTimerTick();
	void localTileUpdate(int count);
	void networkTileUpdate(int count);
	void zoomInButtonClicked();
	void zoomOutButtonClicked();
	void selectComPortButtonClicked();
	void selectComPortSave(QString portname);
	void viewMapsButtonClicked();
	void viewLapTimesButtonClicked();
	void viewGraphsButtonClicked();
	void viewSettingsButtonClicked();

};

#endif // MAINWINDOW_H
