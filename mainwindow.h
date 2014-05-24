#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "irrwidget.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    QirrWidget irrWidget;
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;    
};

#endif // MAINWINDOW_H
