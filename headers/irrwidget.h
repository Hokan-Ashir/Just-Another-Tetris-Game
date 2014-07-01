#ifndef QIRRWIDGET_H
#define QIRRWIDGET_H
#include <QWidget>
#include <QResizeEvent>
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <irrlicht.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
//using namespace io;

class QirrWidget : public QWidget {
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

    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void mousePressEvent(QMouseEvent* event);

    virtual void mouseReleaseEvent(QMouseEvent* event);
    
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void wheelEvent(QWheelEvent* event);

    //тип драйвера, с помощью которого будет создано устройство Иррлихта
    E_DRIVER_TYPE driverType;
    //указатель на устройство Иррлихта
    IrrlichtDevice *device;
    //камера сцены
    ICameraSceneNode* camera;

private:
    
    struct SIrrlichtKey {
        irr::EKEY_CODE code;
        wchar_t ch;
    };

    SIrrlichtKey convertToIrrlichtKey(int key);

    void sendKeyEventToIrrlicht(QKeyEvent* event, bool pressedDown);
    void sendMouseEventToIrrlicht(QMouseEvent* event, bool pressedDown);
    
    irr::s32 lastFPS;
};
#endif // QIRRWIDGET_H
