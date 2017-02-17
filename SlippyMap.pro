#-------------------------------------------------
#
# Project created by QtCreator 2015-04-24T06:49:57
#
#-------------------------------------------------

QT       += core gui network printsupport serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SlippyMap
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tile.cpp \
    map.cpp \
    qcustomplot.cpp \
    logdatasource.cpp \
    datasource.cpp \
    nmeadatasource.cpp \
    tilecache.cpp \
    gpsd.cpp \
    gpsddatasource.cpp \
    jsondatasource.cpp \
    comportselectdialog.cpp

HEADERS  += mainwindow.h \
    tile.h \
    map.h \
    functions.h \
    qcustomplot.h \
    logdatasource.h \
    datasource.h \
    nmeadatasource.h \
    tilecache.h \
    gpsd.h \
    gpsddatasource.h \
    jsondatasource.h \
    comportselectdialog.h

FORMS    += mainwindow.ui \
    comportselectdialog.ui
