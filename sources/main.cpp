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

void createFugureXmlFile(IrrlichtDevice* device) {
    io::path fileName = "rlblock.xml";


    irr::io::IXMLWriter* xwriter = device->getFileSystem()->createXMLWriter(fileName);
    if (!xwriter) {
        irr::io::IWriteFile* file = device->getFileSystem()->createAndWriteFile(fileName);
        if (!file) {
            return;
        }

        xwriter = device->getFileSystem()->createXMLWriter(file);
        if (!xwriter) {
            return;
        }
    }

    //write out the obligatory xml header. Each xml-file needs to have exactly one of those.
    xwriter->writeXMLHeader();

    //start element mygame, you replace the label "mygame" with anything you want
    xwriter->writeElement(L"figure", false, L"name", L"rlblock", L"comment", L"typical reversed lblock node");
    xwriter->writeLineBreak(); //new line

    //start section with video settings
    xwriter->writeElement(L"side", false, L"type", L"up");
    xwriter->writeLineBreak(); //new line
    


    xwriter->writeElement(L"side", false, L"type", L"up");
    xwriter->writeLineBreak(); //new line
    
        xwriter->writeClosingTag(L"side");
    xwriter->writeLineBreak();
    

    xwriter->writeClosingTag(L"side");
    xwriter->writeLineBreak();

    xwriter->writeElement(L"side", false, L"type", L"left");
    xwriter->writeLineBreak(); //new line

    //xwriter->writeElement(L"side", false, L"type", L"right");
    //xwriter->writeLineBreak(); //new line

    //xwriter->writeClosingTag(L"side");
    //xwriter->writeLineBreak();

    xwriter->writeClosingTag(L"side");
    xwriter->writeLineBreak();

    //close mygame section
    xwriter->writeClosingTag(L"figure");

    //delete xml writer
    xwriter->drop();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // Инициализируем наш виджет
    w.irrWidget.init();
    //createFugureXmlFile(w.irrWidget.getIrrlichtDevice());
    
    return a.exec();
}
