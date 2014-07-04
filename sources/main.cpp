#include "headers/mainwindow.h"
#include "headers/CGridSceneNode.h"
#include "headers/CFigure.h"
#include "headers/CMoveByStepAnimator.h"
#include "headers/CMoveDownAnimator.h"
#include "headers/CGameManager.h"
#include "headers/CUserInputEventReceiver.h"
#include "headers/CGameApplication.h"

#include <QtGui/QApplication>
#include <iostream>
using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.irrWidget.init();
    
    return a.exec();
}
