#-------------------------------------------------
#
# Project created by QtCreator 2017-05-16T10:18:01
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpaceXocr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sxoqplaintext.cpp \
    chartdialog.cpp

HEADERS  += mainwindow.h \
    sxoqplaintext.h \
    chartdialog.h

FORMS    += mainwindow.ui \
    chartdialog.ui
