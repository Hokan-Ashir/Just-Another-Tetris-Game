#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // Инициализируем наш виджет
    w.irrWidget.init();

    ISceneManager *smgr = w.irrWidget.getIrrlichtDevice()->getSceneManager();
    IVideoDriver *driver = w.irrWidget.getIrrlichtDevice()->getVideoDriver();
    // Загружаем меш
    IAnimatedMesh* mesh = smgr->getMesh("/home/hokan/Irrlicht/irrlicht-1.8/media/sydney.md2");
    if (!mesh)
    {
        w.irrWidget.getIrrlichtDevice()->drop();
        return 1;
    }
    // Добавляем узел сцены, отключаем освещение, задаем анимацию и накладываем текстуру
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
    if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture( 0, driver->getTexture("/home/hokan/Irrlicht/irrlicht-1.8/media/sydney.bmp") );
    }

    return a.exec();
}
