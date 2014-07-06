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

/**
 * Irrlicht widget, includes device initialization, game aplication initialization, wrappers for Irrlicht events and render/game loop
 */
class QirrWidget : public QWidget {    
    Q_OBJECT
public:
    /**
     * Constructor
     * 
     * @param parent - Qt pernt widget
     */
    explicit QirrWidget(QWidget *parent = 0);
    
    /**
     * Destructor, drops Irrlicht device and game application
     */
    ~QirrWidget();    
    
    /**
     * Getter
     * 
     * @return pointer to Irrlicht device
     */
    IrrlichtDevice* getIrrlichtDevice();

    /**
     * Initialize Irrlicht device, game application and timer loop
     */
    void init();
signals:    
    /**
     * Signal to update Irrlicht, emits from timerEvent and paintEvent
     * 
     * @param device - Irrlicht device to update
     */
    void updateIrrlichtQuery(IrrlichtDevice* device);
public slots:    
    
    /**
     * Slot to update Irrlicht, increase timer, render one loop, call game application one loop
     * 
     * @param device - Irrlicht device to update
     */
    void updateIrrlicht(IrrlichtDevice* device);
protected:    
    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void paintEvent(QPaintEvent* event);
    
    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void timerEvent(QTimerEvent* event);

    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void resizeEvent(QResizeEvent* event);

    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void keyReleaseEvent(QKeyEvent *event);

    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void keyPressEvent(QKeyEvent *event);

    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void mousePressEvent(QMouseEvent* event);

    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void mouseReleaseEvent(QMouseEvent* event);
    
    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void mouseMoveEvent(QMouseEvent *event);

    /**
     * Documentation for all virtual overriden methods see in Qt include files
     */
    virtual void wheelEvent(QWheelEvent* event);
    
    /**
     * render driver type
     */
    irr::video::E_DRIVER_TYPE driverType;
    
    /**
     * Irrlicht device
     */
    IrrlichtDevice *device;

private:
    /**
     * game application
     */
    CGameApplication* gameApplication;
    
    /**
     * struct for convert and passing Qt event to Irrlicht
     */
    struct SIrrlichtKey {
        irr::EKEY_CODE code;
        wchar_t ch;
    };

    /**
     * Convert Qt key to Irrlicht event key
     * 
     * @param key - Qt key to convert
     * @return Irrlicht key and its char code wrapped in structure
     */
    SIrrlichtKey convertToIrrlichtKey(int key);

    /**
     * Sends Qt key event to Irrlicht
     * 
     * @param event - Qt key event to pass
     * @param pressedDown - true, if key is down, false otherwise
     */
    void sendKeyEventToIrrlicht(QKeyEvent* event, bool pressedDown);
    
    /**
     * Sends Qt mouse event to Irrlicht
     * 
     * @param event - Qt mouse event to pass
     * @param pressedDown - true, if key is down, false otherwise
     */
    void sendMouseEventToIrrlicht(QMouseEvent* event, bool pressedDown);
};
#endif // QIRRWIDGET_H
