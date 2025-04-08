#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <QActionGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QToolBar* gatesToolBar = ui->toolBar;

    QAction* andGate = ui->actionAndGate;

    QAction* orGate = ui->actionOrGate;

    QAction* inverter = ui->actionInverter;

    QAction* wire = ui->actionWire;

    QActionGroup *group = new QActionGroup(this);

    group->addAction(andGate);
    group->addAction(orGate);
    group->addAction(inverter);
    group->addAction(wire);
}

MainWindow::~MainWindow()
{
    delete ui;
}
