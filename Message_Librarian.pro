#-------------------------------------------------
#
# Project created by QtCreator 2014-10-14T06:31:07
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Message_Librarian
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingswindow.cpp \
    databasesupport.cpp \
    editsermon.cpp \
    statusindicatordelegate.cpp \
    findsermon.cpp \
    sermonsortfilterproxymodel.cpp \
    publishsermon.cpp

HEADERS  += mainwindow.h \
    settingswindow.h \
    databasesupport.h \
    editsermon.h \
    statusindicatordelegate.h \
    findsermon.h \
    sermonsortfilterproxymodel.h \
    publishsermon.h

FORMS    += mainwindow.ui \
    settingswindow.ui \
    editsermon.ui \
    findsermon.ui \
    publishsermon.ui

DISTFILES += \
    Future_Developement_Notes.txt

RESOURCES += \
    graphicsandsounds.qrc
