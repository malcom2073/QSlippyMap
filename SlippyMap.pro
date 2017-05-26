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
    tilecache.cpp

HEADERS  += mainwindow.h \
    tile.h \
    map.h \
    functions.h \
    tilecache.h

FORMS    += mainwindow.ui
