#-------------------------------------------------
#
# Project created by QtCreator 2014-10-14T06:31:07
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Audio_Sermon_Organizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingswindow.cpp

HEADERS  += mainwindow.h \
    settingswindow.h

FORMS    += mainwindow.ui \
    settingswindow.ui
