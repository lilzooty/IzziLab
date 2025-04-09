#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <QActionGroup>
#include <QPushButton>
#include "draggablebutton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), andGates{}, orGates{}, inverters{}
{
    ui->setupUi(this);

    QAction* andGate = ui->actionAndGate;

    QAction* orGate = ui->actionOrGate;

    QAction* inverter = ui->actionInverter;

    QAction* wire = ui->actionWire;

    QActionGroup *group = new QActionGroup(this);

    group->addAction(andGate);
    group->addAction(orGate);
    group->addAction(inverter);
    group->addAction(wire);

    connect(ui->actionAndGate, &QAction::triggered, this, &MainWindow::onAndGateClicked);
    connect(ui->actionOrGate, &QAction::triggered, this, &MainWindow::onOrGateClicked);
    connect(ui->actionInverter, &QAction::triggered, this, &MainWindow::onInverterClicked);
    connect(ui->actionWire, &QAction::triggered, this, &MainWindow::onWireClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAndGateClicked()
{
    andGates.push_back(new DraggableButton("AND", this));
    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
}

void MainWindow::onOrGateClicked()

{
    orGates.push_back(new DraggableButton("OR", this));
    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
}

void MainWindow::onInverterClicked()
{
    inverters.push_back(new DraggableButton("NOT", this));
    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
}

void MainWindow::onWireClicked()
{
    return;
}
