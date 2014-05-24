#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T16:52:32
#
#-------------------------------------------------

QT       += core gui

TARGET = TetrisGame
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    irrwidget.cpp

HEADERS  += mainwindow.h \
    irrwidget.h

FORMS    += mainwindow.ui
LIBS += -L/home/hokan/Irrlicht/irrlicht-1.8/lib/Linux -lIrrlicht -lGL -lX11 -lXxf86vm
INCLUDEPATH += /home/hokan/Irrlicht/irrlicht-1.8/include
