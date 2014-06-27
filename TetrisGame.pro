#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T16:52:32
#
#-------------------------------------------------

QT       += core gui

TARGET = TetrisGame
TEMPLATE = app


SOURCES += sources/main.cpp\
        sources/mainwindow.cpp \
    sources/irrwidget.cpp \
    sources/CGridSceneNode.cpp

HEADERS  += headers/mainwindow.h \
    headers/irrwidget.h \
    headers/CGridSceneNode.h

FORMS    += forms/mainwindow.ui
LIBS += -L/home/hokan/Irrlicht/irrlicht-1.8/lib/Linux -lIrrlicht -lGL -lX11 -lXxf86vm
INCLUDEPATH += /home/hokan/Irrlicht/irrlicht-1.8/include
