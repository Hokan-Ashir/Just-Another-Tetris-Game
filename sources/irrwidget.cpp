#include "headers/irrwidget.h"
#include "headers/CGameApplication.h"

#include <iostream>

QirrWidget::QirrWidget(QWidget *parent) :
QWidget(parent) {
    setAttribute(Qt::WA_PaintOnScreen);

    device = NULL;
    driverType = irr::video::EDT_OPENGL;
    grabKeyboard();

    lastFPS = -1;
}

QirrWidget::~QirrWidget() {
    //delete gameApplication;

    if (device != NULL) {
        device->closeDevice();
        device->drop();
    }
}

void QirrWidget::init() {
    if (device != NULL) {
        return;
    }

    SIrrlichtCreationParameters params;
    params.Bits = 32;
    params.Doublebuffer = true;
    params.DriverType = driverType;
    params.EventReceiver = 0;
    params.Fullscreen = false;
    params.IgnoreInput = false;
    params.Stencilbuffer = true;
    params.Stereobuffer = false;
    params.WindowId = reinterpret_cast<void*> (this->winId());
    params.WindowSize.Width = width();
    params.WindowSize.Height = height();

    device = createDeviceEx(params);

    device->getLogger()->setLogLevel(irr::ELL_INFORMATION);

    setAttribute(Qt::WA_OpaquePaintEvent);

    connect(this, SIGNAL(updateIrrlichtQuery(IrrlichtDevice*)), this, SLOT(updateIrrlicht(IrrlichtDevice*)));

    gameApplication = new CGameApplication();
    if (!gameApplication->initialize(device)) {
        return;
    }

    startTimer(0);
}

IrrlichtDevice* QirrWidget::getIrrlichtDevice() {
    return device;
}

void QirrWidget::paintEvent(QPaintEvent* event) {
    if (device != NULL) {
        emit updateIrrlichtQuery(device);
    }
}

QirrWidget::SIrrlichtKey QirrWidget::convertToIrrlichtKey(int key) {
    SIrrlichtKey irrKey;
    irrKey.code = (irr::EKEY_CODE)(0);
    irrKey.ch = (wchar_t)(0);

    // Letters A..Z and numbers 0..9 are mapped directly
    if ((key >= Qt::Key_A && key <= Qt::Key_Z) || (key >= Qt::Key_0 && key <= Qt::Key_9)) {
        irrKey.code = (irr::EKEY_CODE)(key);
        irrKey.ch = (wchar_t)(key);
    } else

        // Dang, map keys individually
        switch (key) {
            case Qt::Key_Up:
                irrKey.code = irr::KEY_UP;
                break;

            case Qt::Key_Down:
                irrKey.code = irr::KEY_DOWN;
                break;

            case Qt::Key_Left:
                irrKey.code = irr::KEY_LEFT;
                break;

            case Qt::Key_Right:
                irrKey.code = irr::KEY_RIGHT;
                break;

            case Qt::Key_Space:
                irrKey.code = irr::KEY_SPACE;
                break;

            case Qt::Key_Plus:
                irrKey.code = irr::KEY_PLUS;
                break;

            case Qt::Key_Minus:
                irrKey.code = irr::KEY_MINUS;
                break;
        }
    return irrKey;
}

void QirrWidget::sendKeyEventToIrrlicht(QKeyEvent* event, bool pressedDown) {
    irr::SEvent irrEvent;

    irrEvent.EventType = irr::EET_KEY_INPUT_EVENT;

    SIrrlichtKey irrKey = convertToIrrlichtKey(event->key());

    if (irrKey.code == 0) return; // Could not find a match for this key

    irrEvent.KeyInput.Key = irrKey.code;
    irrEvent.KeyInput.Control = ((event->modifiers() & Qt::ControlModifier) != 0);
    irrEvent.KeyInput.Shift = ((event->modifiers() & Qt::ShiftModifier) != 0);
    irrEvent.KeyInput.Char = irrKey.ch;
    irrEvent.KeyInput.PressedDown = pressedDown;

    device->postEventFromUser(irrEvent);
}

void QirrWidget::keyReleaseEvent(QKeyEvent *event) {
    if (device != NULL) {
        sendKeyEventToIrrlicht(event, false);
    }
    event->ignore();
}

void QirrWidget::keyPressEvent(QKeyEvent *event) {
    if (device != NULL) {
        sendKeyEventToIrrlicht(event, true);
    }
    event->ignore();
}

void QirrWidget::sendMouseEventToIrrlicht(QMouseEvent* event, bool pressedDown) {
    irr::SEvent irrEvent;

    irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

    switch (event->button()) {
        case Qt::LeftButton:
            irrEvent.MouseInput.Event = pressedDown ? irr::EMIE_LMOUSE_PRESSED_DOWN : irr::EMIE_LMOUSE_LEFT_UP;
            break;

        case Qt::MidButton:
            irrEvent.MouseInput.Event = pressedDown ? irr::EMIE_MMOUSE_PRESSED_DOWN : irr::EMIE_MMOUSE_LEFT_UP;
            break;

        case Qt::RightButton:
            irrEvent.MouseInput.Event = pressedDown ? irr::EMIE_RMOUSE_PRESSED_DOWN : irr::EMIE_RMOUSE_LEFT_UP;
            break;

        default:
            return; // Cannot handle this mouse event
    }

    irrEvent.MouseInput.X = event->x();
    irrEvent.MouseInput.Y = event->y();
    irrEvent.MouseInput.Wheel = 0.0f; // Zero is better than undefined    

    device->postEventFromUser(irrEvent);
}

void QirrWidget::mousePressEvent(QMouseEvent* event) {
    if (device != NULL) {
        sendMouseEventToIrrlicht(event, true);
    }
    event->ignore();
}

void QirrWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (device != NULL) {
        sendMouseEventToIrrlicht(event, false);
    }
    event->ignore();
}

void QirrWidget::mouseMoveEvent(QMouseEvent *event) {
    if (device != NULL) {
        irr::SEvent irrEvent;

        irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

        irrEvent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
        irrEvent.MouseInput.X = event->x();
        irrEvent.MouseInput.Y = event->y();
        irrEvent.MouseInput.Wheel = 0.0f; // Zero is better than undefined        
        device->postEventFromUser(irrEvent);
    }
    event->ignore();
}

void QirrWidget::wheelEvent(QWheelEvent* event) {
    if (device != NULL && event->orientation() == Qt::Vertical) {
        irr::SEvent irrEvent;

        irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

        irrEvent.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
        irrEvent.MouseInput.X = 0; // We don't know these,
        irrEvent.MouseInput.Y = 0; // but better zero them instead of letting them be undefined
        irrEvent.MouseInput.Wheel = event->delta() / 120.0f;

        device->postEventFromUser(irrEvent);
    }
    event->ignore();
}

void QirrWidget::timerEvent(QTimerEvent* event) {
    if (device != NULL) {
        emit updateIrrlichtQuery(device);
    }
    event->accept();
}

void QirrWidget::resizeEvent(QResizeEvent* event) {
    if (device != NULL) {
        irr::core::dimension2d<unsigned int> size;
        if (event->size().height() < event->size().width()) {
            size.Width = event->size().height();
            size.Height = event->size().height();
        } else if (event->size().height() > event->size().width()) {
            size.Width = event->size().width();
            size.Height = event->size().width();
        } else if (event->size().height() == event->size().width()) {
            size.Width = event->size().width();
            size.Height = event->size().height();
        }
        device->getVideoDriver()->OnResize(size);
        resize(size.Width, size.Height);

        irr::scene::ICameraSceneNode *cam = device->getSceneManager()->getActiveCamera();
        if (cam != 0) {
            cam->setAspectRatio(size.Height / size.Width);
        }
    }
    QWidget::resizeEvent(event);
}

void QirrWidget::updateIrrlicht(irr::IrrlichtDevice* device) {
    if (device != NULL) {
        device->getTimer()->tick();

        irr::video::SColor color(0, 0, 0, 0);

        device->getVideoDriver()->beginScene(true, true, color);

        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();

        device->getVideoDriver()->endScene();

        if (!gameApplication->gameLoopIteration()) {
            // game is over
            //delete gameApplication;

            /*if (device != 0) {
                device->closeDevice();
                device->drop();
            }*/
        }
    }
}
