#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <QActionGroup>
#include <QPushButton>
#include "draggablebutton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
    // DraggableButton* button = new DraggableButton("AND", this);
    // QPoint globalMousePos = QCursor::pos();
    // QPoint widgetPos = this->mapFromGlobal(globalMousePos);
}

void MainWindow::onOrGateClicked()
{
    QPushButton* button = new QPushButton("OR", this);

    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);

    button->move(widgetPos);
    button->show();
}

void MainWindow::onInverterClicked()
{
    QPushButton* button = new QPushButton("NOT", this);

    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);

    button->move(widgetPos);
    button->show();
}

void MainWindow::onWireClicked()
{
    return;
}
