#include "headers/irrwidget.h"

#include <iostream>

QirrWidget::QirrWidget(QWidget *parent) :
QWidget(parent) {
    setAttribute(Qt::WA_PaintOnScreen);

    device = 0;
    // задаем тип драйвера
    driverType = EDT_OPENGL;

    grabKeyboard();
    
    //setMouseTracking(true);
    
    //TODO focus policy
    //setFocusPolicy(Qt::StrongFocus);
    //grabMouse();
}

QirrWidget::~QirrWidget() {
    if (device != 0) {
        device->closeDevice();
        device->drop();
    }
}

void QirrWidget::init() {
    //Инициализация должна быть только один раз
    if (device != 0)
        return;
    //параметры для создания устройства
    SIrrlichtCreationParameters params;
    params.Bits = 32;
    params.Doublebuffer = true;
    //тип драйвера
    params.DriverType = driverType;
    params.EventReceiver = 0;
    params.Fullscreen = false;
    params.IgnoreInput = false;
    params.Stencilbuffer = true;
    params.Stereobuffer = false;
    //указатель на окно в котором будет выводиться графика
    params.WindowId = reinterpret_cast<void*> (this->winId());
    //Ширина и высота окна Иррлихта равны высоте и ширине виджета
    params.WindowSize.Width = width();
    params.WindowSize.Height = height();

    //Создание устройства по заданным параметрам
    device = createDeviceEx(params);

    QString c = QString("width %1, height %2").arg(width()).arg(height());
    device->getLogger()->setLogLevel(irr::ELL_INFORMATION);
    device->getLogger()->log(c.toUtf8().data());
    // qDebug() << "widget ID " << showbase << hex << winId();
    //qDebug() << "win ID " << params.WindowId;

    //Добавим на сцену статическую камеру
    //camera = device->getSceneManager()->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));
    //задаем атрибут для виджета
    //значение WA_OpaquePaintEvent указывает, что виджет рисует все его пиксели, когда он получает событие рисования
    setAttribute(Qt::WA_OpaquePaintEvent);
    //связываем сигнал с нужным слотом
    //1 параметр - указатель отправителя сигнала
    //2 параметр - функция сигнала
    //3 параметр - указатель на получателя сигнала
    //4 параметр - слот. Функция, вызываемая в ответ на сигнал
    connect(this, SIGNAL(updateIrrlichtQuery(IrrlichtDevice*)), this, SLOT(updateIrrlicht(IrrlichtDevice*)));
    //Запускает таймер с заданным интревалом.
    //Если интервал равен 0, тогда событие таймера происходит один раз, когда
    //для обработки нет больше событий окна.
    startTimer(0);
}

IrrlichtDevice* QirrWidget::getIrrlichtDevice() {
    return device;
}

void QirrWidget::paintEvent(QPaintEvent* event) {
    if (device != 0) {
        //посылаем сигнал запроса на обновление кадра
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
    if (device != 0) {
        sendKeyEventToIrrlicht(event, false);
        device->getLogger()->log(event->text().toUtf8().data());
    }
    event->ignore();
}

void QirrWidget::keyPressEvent(QKeyEvent *event) {
    if (device != 0) {
        sendKeyEventToIrrlicht(event, true);
        device->getLogger()->log(event->text().toUtf8().data());
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

    QString mouseString = QString("x %1, y %2").arg(event->x()).arg(event->y());
    device->getLogger()->log(mouseString.toUtf8().data());

    device->postEventFromUser(irrEvent);
}

void QirrWidget::mousePressEvent(QMouseEvent* event) {
    if (device != 0) {
        sendMouseEventToIrrlicht(event, true);
        //device->getLogger()->log(event->text().toUtf8().data());
    }
    event->ignore();
}

void QirrWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (device != 0) {
        sendMouseEventToIrrlicht(event, false);
        //device->getLogger()->log(event->button()toUtf8().data());
    }
    event->ignore();
}

void QirrWidget::mouseMoveEvent(QMouseEvent *event) {
    if (device != 0) {
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
    if (device != 0 && event->orientation() == Qt::Vertical) {
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
    if (device != 0) {
        //посылаем сигнал запроса на обновление кадра
        emit updateIrrlichtQuery(device);
    }
    event->accept();
}

void QirrWidget::resizeEvent(QResizeEvent* event) {
    if (device != 0) {
        //Для задаем новые размеры окна вывода в соотвествии с новым размером виджета
        dimension2d<u32> widgetSize;
        widgetSize.Width = event->size().width();
        widgetSize.Height = event->size().height();
        device->getVideoDriver()->OnResize(widgetSize);
        ICameraSceneNode *cam = device->getSceneManager()->getActiveCamera();
        if (cam != 0) {
            //Задаем формат экрана. Соотношение его ширины к высоте
            cam->setAspectRatio((f32) widgetSize.Height / (f32) widgetSize.Width);
        }
    }
    //Вызываем событие изменения размера виджета
    QWidget::resizeEvent(event);
}

void QirrWidget::updateIrrlicht(irr::IrrlichtDevice* device) {
    if (device != 0) {
        device->getTimer()->tick();

        irr::video::SColor color(0, 0, 0, 0);

        device->getVideoDriver()->beginScene(true, true, color);

        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();

        device->getVideoDriver()->endScene();
    }
}
