#include "irrwidget.h"
QirrWidget::QirrWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);

    device = 0;
    // задаем тип драйвера
    driverType = EDT_OPENGL;
}
QirrWidget::~QirrWidget()
{
    if ( device != 0 )
    {
        device->closeDevice();
        device->drop();
    }
}
void QirrWidget::init()
{
    //Инициализация должна быть только один раз
    if ( device != 0 )
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
    params.WindowId = reinterpret_cast<void*>(this->winId());
    //Ширина и высота окна Иррлихта равны высоте и ширине виджета
    params.WindowSize.Width = width();
    params.WindowSize.Height = height();
    //Создание устройства по заданным параметрам
    device = createDeviceEx(params);
    //Добавим на сцену статическую камеру
    camera = device->getSceneManager()->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
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
IrrlichtDevice* QirrWidget::getIrrlichtDevice()
{
    return device;
}
void QirrWidget::paintEvent(QPaintEvent* event)
{
    if ( device != 0 )
    {
        //посылаем сигнал запроса на обновление кадра
        emit updateIrrlichtQuery(device);
    }
}
void QirrWidget::timerEvent(QTimerEvent* event)
{
    if ( device != 0 )
    {
        //посылаем сигнал запроса на обновление кадра
        emit updateIrrlichtQuery(device);
    }
    event->accept();
}
void QirrWidget::resizeEvent(QResizeEvent* event)
{
    if ( device != 0 )
    {
        //Для задаем новые размеры окна вывода в соотвествии с новым размером виджета
        dimension2d<u32> widgetSize;
        widgetSize.Width = event->size().width();
        widgetSize.Height = event->size().height();
        device->getVideoDriver()->OnResize(widgetSize);
        ICameraSceneNode *cam = device->getSceneManager()->getActiveCamera();
        if (cam != 0)
        {
            //Задаем формат экрана. Соотношение его ширины к высоте
            cam->setAspectRatio( (f32)widgetSize.Height / (f32)widgetSize.Width );
        }
    }
    //Вызываем событие изменения размера виджета
    QWidget::resizeEvent(event);
}
void QirrWidget::updateIrrlicht( irr::IrrlichtDevice* device )
{
    if(device != 0)
    {
        //Обновим таймер
        device->getTimer()->tick();
        SColor color (255,100,101,140);
        device->getVideoDriver()->beginScene(true, true, color);
            device->getSceneManager()->drawAll();
        device->getVideoDriver()->endScene();
    }
}
