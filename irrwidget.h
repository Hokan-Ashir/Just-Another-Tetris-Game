#ifndef QIRRWIDGET_H
#define QIRRWIDGET_H
#include <QWidget>
#include <QResizeEvent>
#include "irrlicht.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
class QirrWidget : public QWidget
{
    //макрос для мета-объектного компилятора(MOC). Подробнее в документации
    Q_OBJECT
public:
    explicit QirrWidget(QWidget *parent = 0);
    ~QirrWidget();
    //функция получения указателя на устройство Иррлихта
    IrrlichtDevice* getIrrlichtDevice();
    //функция инициализации движка виджета
    void init();
signals:
    //сигнал, который делает запрос на обновление кадра
    void updateIrrlichtQuery(IrrlichtDevice* device);
public slots:
    //функция, которая вызывается в ответ на сигнал, который делает запрос на обновление кадра
    void updateIrrlicht(IrrlichtDevice* device);
protected:
    //переопределенный метод для получения события перерисовки виджета
    virtual void paintEvent(QPaintEvent* event);
    //переопределенный метод для получения события таймера для объекта
    virtual void timerEvent(QTimerEvent* event);
    //переопределенный метод для получения события изменения размера виджета
    virtual void resizeEvent(QResizeEvent* event);
    //тип драйвера, с помощью которого будет создано устройство Иррлихта
    E_DRIVER_TYPE driverType;
    //указатель на устройство Иррлихта
    IrrlichtDevice *device;
    //камера сцены
    ICameraSceneNode* camera;
};
#endif // QIRRWIDGET_H
