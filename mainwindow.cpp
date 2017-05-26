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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//39.9628° N, 76.7281° W
	state = 0;
	m_lapCount = 1;
	connect(ui->map,SIGNAL(mouseMoved(double,double)),this,SLOT(mouseMoved(double,double)));


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

	return;
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
