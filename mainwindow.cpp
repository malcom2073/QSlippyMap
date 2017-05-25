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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include "tile.h"
#include <QDebug>
#include <QFile>
#include "nmeadatasource.h"
#include "jsondatasource.h"
#include "comportselectdialog.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//39.9628° N, 76.7281° W
	m_logDataSource = 0;
	state = 0;
	m_lapCount = 1;
	connect(ui->map,SIGNAL(mouseMoved(double,double)),this,SLOT(mouseMoved(double,double)));

	m_logDataSource = new JSONDataSource();
	connect(m_logDataSource,SIGNAL(incomingData(DataClass)),this,SLOT(incomingData(DataClass)));

	//ui->plot->addGraph();
	//ui->plot->graph(0)->setPen(QColor::fromRgb(255,0,0));
	//ui->plot->xAxis->setRange(0,70);
	//ui->plot->yAxis->setRange(0,100);

	connect(ui->replayPushButton,SIGNAL(clicked()),this,SLOT(replayClicked()));

	connect(ui->comPortPushButton,SIGNAL(clicked()),this,SLOT(selectComPortButtonClicked()));


	//Mark's house
	//m_centerLat = 36.56432333;
	//m_centerLon = -79.20867920;
	//39.1559 -76.5353
	m_totalAngle = 0;

	//NJMR
	m_centerLat = 39.361947;
	m_centerLon = -75.072810;


	//NMEADataSource *source = new NMEADataSource();
	//connect(source,SIGNAL(incomingData(DataClass)),this,SLOT(incomingData(DataClass)));
	//source->start();
	connect(ui->map,SIGNAL(mouseReleased(double,double)),this,SLOT(mouseClicked(double,double)));
	ui->tableWidget->setColumnCount(3);
	ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Lap"));
	ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Time"));
	m_lapTimeUpdateTimer = new QTimer(this);
	connect(m_lapTimeUpdateTimer,SIGNAL(timeout()),this,SLOT(lapTimeUpdateTimerTick()));
	// m_lapTimeUpdateTimer->start(500);
	m_startDateTime = QTime::currentTime();
	m_lapStartDateTime = QTime::currentTime();
	m_startDateTime.start();
	m_lapStartDateTime.start();

	m_tileUpdateLabel = new QLabel(this);
	ui->statusBar->addWidget(m_tileUpdateLabel);
	m_tileUpdateLabel->show();

	m_tileDownloadLabel = new QLabel(this);
	ui->statusBar->addWidget(m_tileDownloadLabel);
	m_tileDownloadLabel->show();

	connect(ui->map,SIGNAL(networkTileUpdate(int)),this,SLOT(networkTileUpdate(int)));
	connect(ui->map,SIGNAL(localTileUpdate(int)),this,SLOT(localTileUpdate(int)));
	ui->map->setCenter(m_centerLat,m_centerLon,14);


	//ui->plotScrollArea->setLayout(new QVBoxLayout());
	QWidget *widget = new QWidget(ui->plotScrollArea);
	widget->setLayout(new QVBoxLayout());
	ui->plotScrollArea->setWidget(widget);
	//ui->plotScrollArea->widget()->setSizeConstraint(QLayout::SetMinAndMaxSize);

	m_gyroPlot = new QCustomPlot(ui->plotScrollArea->widget());
	m_gyroPlot->addGraph();
	m_gyroPlot->addGraph();
	m_gyroPlot->addGraph();
	QPen pen = m_gyroPlot->graph(0)->pen();
	pen.setColor(QColor::fromRgb(255,0,0));
	m_gyroPlot->graph(0)->setPen(pen);
	pen.setColor(QColor::fromRgb(0,255,0));
	m_gyroPlot->graph(1)->setPen(pen);
	pen.setColor(QColor::fromRgb(0,0,255));
	m_gyroPlot->graph(2)->setPen(pen);
	m_gyroPlot->show();
	m_gyroPlot->setMinimumHeight(300);
	ui->plotScrollArea->widget()->layout()->addWidget(m_gyroPlot);

	m_accelPlot = new QCustomPlot(ui->plotScrollArea->widget());
	m_accelPlot->addGraph();
	m_accelPlot->addGraph();
	m_accelPlot->addGraph();
	pen = m_accelPlot->graph(0)->pen();
	pen.setColor(QColor::fromRgb(255,0,0));
	m_accelPlot->graph(0)->setPen(pen);
	pen.setColor(QColor::fromRgb(0,255,0));
	m_accelPlot->graph(1)->setPen(pen);
	pen.setColor(QColor::fromRgb(0,0,255));
	m_accelPlot->graph(2)->setPen(pen);
	m_accelPlot->show();
	m_accelPlot->setMinimumHeight(300);
	ui->plotScrollArea->widget()->layout()->addWidget(m_accelPlot);
}

MainWindow::~MainWindow()
{
	delete ui;
}
void MainWindow::mouseClicked(double lat, double lon)
{
	//m_centerLat = lat;
	//m_centerLon = lon;
	qDebug() << "Mouse Clicked Lat:" << lat << "Lon:" << lon;
}

void MainWindow::mouseMoved(double lat,double lon)
{
	//this->setWindowTitle(QString::number(lat,'f',8) + "," + QString::number(lon,'f',8) + "," + QString::number(m_totalAngle,'f',2));
}
void MainWindow::replayClicked()
{
	//m_logDataSource= new LogDataSource();
	//connect(m_logDataSource,SIGNAL(incomingData(DataClass)),this,SLOT(incomingData(DataClass)));
	//m_logDataSource->loadFile("logs/1429992670496.two.txt");
	//m_logDataSource->start();

	//m_logDataSource = new GPSDDataSource();

	m_logDataSource->start();
	return;
}
void MainWindow::incomingData(DataClass data)
{
	if (data.imuvalid)
	{
		m_gyroPlot->graph(0)->addData(data.msecs,data.gyrox);
		m_gyroPlot->graph(1)->addData(data.msecs,data.gyroy);
		m_gyroPlot->graph(2)->addData(data.msecs,data.gyroz);
		m_accelPlot->graph(0)->addData(data.msecs,data.accelx);
		m_accelPlot->graph(1)->addData(data.msecs,data.accely);
		m_accelPlot->graph(2)->addData(data.msecs,data.accelz);

		m_gyroPlot->rescaleAxes();
		m_gyroPlot->replot();

		m_accelPlot->rescaleAxes();
		m_accelPlot->replot();
	}
	else if (data.locationvalid)
	{
		double lat = data.latitude;
		double lon = data.longitutde;
		ui->map->centerOn(lat,lon);

		double speed = data.speed;
		ui->map->addWaypoint(lat,lon,speed);
		ui->map->setCurrentPosition(lat,lon);
		//ui->plot->graph(ui->plot->graphCount()-1)->addData(m_calculatedDistance,speed);
		//ui->plot->rescaleAxes();
		//ui->plot->replot();

		double deltalat = m_centerLat - lat;
		double deltalon = m_centerLon - lon;
		double angle = atan2(deltalat,deltalon);
		//this->setWindowTitle(QString::number(m_totalAngle * (180 / M_PI),'f',2));

		if (state == 0)
		{
			if (m_centerLat > lat)
			{
				state = 1;
			}
			else
			{
				state = 2;
			}
			m_lastAngle = angle;
			m_lastPoint = QPointF(lon,lat);
			//state = 1;
		}
		else if (state == 1)
		{
			qDebug() << "1" << m_lastAngle << angle << m_totalAngle;
			m_calculatedDistance += sqrt(abs(pow(m_lastPoint.x() - lon,2) - pow(m_lastPoint.y()-lat,2)));
			if (m_lastAngle < 0 && angle > 0)
			{
				//Rolled over going clockwise
				state = 2;
				m_totalAngle += (M_PI - angle) + (M_PI + m_lastAngle);
				m_totalUnsignedAngle += abs((M_PI - angle) + (M_PI + m_lastAngle));
			}
			else if (m_lastAngle > 0 && angle < 0)
			{
				//Rolled over going counter-clockwise
				state = 2;
				m_totalAngle += m_lastAngle - (2*M_PI + angle);
				m_totalUnsignedAngle += abs(m_lastAngle - ((2*M_PI) + angle));
			}
			else
			{
				m_totalAngle += m_lastAngle - angle;
				m_totalUnsignedAngle += abs(m_lastAngle - angle);
			}
			m_lastAngle = angle;
		}
		else if (state == 2)
		{
			qDebug() << "2" << m_lastAngle << angle << m_totalAngle;
			m_calculatedDistance += sqrt(abs(pow(m_lastPoint.x() - lon,2) - pow(m_lastPoint.y()-lat,2)));
			if (m_lastAngle > 0 && angle < 0)
			{
				state = 1;
				//m_totalAngle += (M_PI + angle) + (M_PI - m_lastAngle);
				m_totalAngle += m_lastAngle - angle;
				m_totalUnsignedAngle += abs(m_lastAngle - angle);
			}
			else if (m_lastAngle < 0 && angle > 0)
			{
				state = 1;
				m_totalAngle += m_lastAngle - angle;
				m_totalUnsignedAngle += abs(m_lastAngle - angle);
			}
			else
			{
				m_totalAngle += m_lastAngle - angle;
				m_totalUnsignedAngle += abs(m_lastAngle - angle);
			}
			m_lastAngle = angle;
		}
		if ((m_totalAngle > (2 * M_PI * m_lapCount)) || (m_totalAngle < (-2 * M_PI * m_lapCount)))
		{
			int lapspan = m_lapStartDateTime.elapsed();
			m_lapStartDateTime.restart();
			int startspan = m_startDateTime.elapsed();
			ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
			ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(QString::number(m_lapCount)));
			ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(QTime::fromMSecsSinceStartOfDay(lapspan).toString()));
			ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(QTime::fromMSecsSinceStartOfDay(startspan).toString()));
			m_lapCount++;
			m_calculatedTotalLapAngle = m_totalUnsignedAngle;
			m_totalUnsignedAngle = 0;
			m_calculatedDistance = 0;
			//ui->plot->addGraph();
			//ui->plot->graph(ui->plot->graphCount()-1)->setPen(QColor::fromRgb(255,0,0));
			//for (int i=0;i<ui->plot->graphCount()-1;i++)
			//{
			//	ui->plot->graph(i)->setPen(QColor::fromRgb(0,0,255));
			//}
		}
		m_lastPoint = QPointF(lon,lat);
		this->setWindowTitle("Lap count:" + QString::number(m_lapCount) + "," + QString::number(m_totalAngle,'f',2) + "," + QString::number(m_calculatedDistance,'f',2));
	}
}
void MainWindow::setStartButtonClicked()
{
	m_totalAngle = 0;
}
void MainWindow::lapTimeUpdateTimerTick()
{
	QTime current = QTime::currentTime();
	int lapspan = m_lapStartDateTime.elapsed();
	int startspan = m_startDateTime.elapsed();

	ui->lapLabel->setText("Lap: " + QTime::fromMSecsSinceStartOfDay(lapspan).toString());
	ui->splitLabel->setText("Split: " + QTime::fromMSecsSinceStartOfDay(startspan).toString());
}
void MainWindow::localTileUpdate(int count)
{
	m_tileUpdateLabel->setText("Tiles to load: " + QString::number(count));
}

void MainWindow::networkTileUpdate(int count)
{
	m_tileDownloadLabel->setText("Tiles to download: " + QString::number(count));
}
void MainWindow::zoomInButtonClicked()
{
	ui->map->setZoom(ui->map->getZoom()+1);
}

void MainWindow::zoomOutButtonClicked()
{
	ui->map->setZoom(ui->map->getZoom()-1);
}
void MainWindow::selectComPortButtonClicked()
{
	ComPortSelectDialog *dialog = new ComPortSelectDialog();
	connect(dialog,SIGNAL(savePort(QString)),this,SLOT(selectComPortSave(QString)));
	dialog->show();
}
void MainWindow::selectComPortSave(QString portname)
{
	ui->comPortPushButton->setText(portname);
	m_logDataSource->setConnectionSpecific("portname",portname);
	qobject_cast<ComPortSelectDialog*>(sender())->deleteLater();
}
void MainWindow::viewMapsButtonClicked()
{
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::viewLapTimesButtonClicked()
{
	ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::viewGraphsButtonClicked()
{
	ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::viewSettingsButtonClicked()
{
	ui->stackedWidget->setCurrentIndex(3);
}
