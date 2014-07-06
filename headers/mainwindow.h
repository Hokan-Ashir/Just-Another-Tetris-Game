#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "irrwidget.h"


#include <QMainWindow>

namespace Ui {
class MainWindow;
}

/**
 * Wrapper for game window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    /**
     * Constructot
     * 
     * @param parent - parent Qt widget
     */
    explicit MainWindow(QWidget *parent = 0);
    
    /**
     * Irrlicht widget
     */
    QirrWidget irrWidget;
    
    /**
     * Destructor, not implemented
     */
    ~MainWindow();
    
private:
    /**
     * widget form pointer
     */
    Ui::MainWindow *ui;    
};

#endif // MAINWINDOW_H
