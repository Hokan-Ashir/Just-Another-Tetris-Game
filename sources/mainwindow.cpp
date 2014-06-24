#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(&irrWidget);
    //irrWidget.setParent(ui->centralWidget);
    //irrWidget.setGeometry(160, 0, 630, 480);
}

MainWindow::~MainWindow()
{
    delete ui;
}
