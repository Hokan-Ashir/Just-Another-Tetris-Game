#ifndef QIRRWIDGET_H
#define QIRRWIDGET_H

#include "CGameApplication.h"

#include <QWidget>
#include <QResizeEvent>
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <irrlicht.h>
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"

using namespace irr;

class QirrWidget : public QWidget {    
    Q_OBJECT
public:
    explicit QirrWidget(QWidget *parent = 0);
    ~QirrWidget();    
    IrrlichtDevice* getIrrlichtDevice();    
    void init();
signals:    
    void updateIrrlichtQuery(IrrlichtDevice* device);
public slots:    
    void updateIrrlicht(IrrlichtDevice* device);
protected:    
    virtual void paintEvent(QPaintEvent* event);
    
    virtual void timerEvent(QTimerEvent* event);

    virtual void resizeEvent(QResizeEvent* event);

    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void mousePressEvent(QMouseEvent* event);

    virtual void mouseReleaseEvent(QMouseEvent* event);
    
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void wheelEvent(QWheelEvent* event);
    
    irr::video::E_DRIVER_TYPE driverType;
    
    IrrlichtDevice *device;

private:
    CGameApplication* gameApplication;
    
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
