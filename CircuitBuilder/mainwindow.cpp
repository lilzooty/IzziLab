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

    //physics set up
    // Initialize physics
    initializePhysics();

    // Setup physics update timer
    physicsTimer = new QTimer(this);
    connect(physicsTimer, &QTimer::timeout, this, &MainWindow::updatePhysics);
    physicsTimer->start(16); // 60 FPS approximately

}

MainWindow::~MainWindow()
{
    if(physicsWorld){
        delete physicsWorld;
    }
    delete ui;
}

void MainWindow::onAndGateClicked()
{
    DraggableButton* newButton = new DraggableButton("AND", this);
    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
    newButton->setPosition(widgetPos);
    newButton->setGeometry(widgetPos.x(), widgetPos.y(), GATE_SIZE, GATE_SIZE); // Set initial size
    newButton->show();
    andGates.push_back(newButton);
    createPhysicsBody(newButton);
}

void MainWindow::onOrGateClicked()
{
    DraggableButton* newButton = new DraggableButton("OR", this);
    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
    newButton->setPosition(widgetPos);
    newButton->setGeometry(widgetPos.x(), widgetPos.y(), GATE_SIZE, GATE_SIZE); // Set initial size
    newButton->show();
    orGates.push_back(newButton);
    createPhysicsBody(newButton);
}

void MainWindow::onInverterClicked()
{
    DraggableButton* newButton = new DraggableButton("NOT", this);
    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
    newButton->setPosition(widgetPos);
    newButton->setGeometry(widgetPos.x(), widgetPos.y(), GATE_SIZE, GATE_SIZE); // Set initial size
    newButton->show();
    inverters.push_back(newButton);
    createPhysicsBody(newButton);
}

void MainWindow::onWireClicked()
{
    return;
}
void MainWindow::createPhysicsBody(DraggableButton* button)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    QPoint ps = button->getPosition();
    bodyDef.position.Set(ps.x() / pixelsPerMeter, ps.y() / pixelsPerMeter);

    b2Body* body = physicsWorld->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(25.0f / pixelsPerMeter, 25.0f / pixelsPerMeter);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;

    body->CreateFixture(&fixtureDef);
    button->setProperty("physicsBody", QVariant::fromValue((void*)body));
    button->setBody(body);
}

void MainWindow::initializePhysics()
{
    physicsWorld = new b2World(b2Vec2(.0f, .0f));
}

void MainWindow::updatePhysics()
{
    // Step the physics simulation
    physicsWorld->Step(1.0f/60.0f, 6, 2);

    // Update all buttons positions
    auto updateButtons = [this](const vector<DraggableButton*>& buttons) {
        for (auto button : buttons) {
            if (b2Body* body = (b2Body*)button->property("physicsBody").value<void*>()) {
                b2Vec2 position = body->GetPosition();
                // Convert physics coordinates to screen coordinates
                QPoint screenPos(position.x * pixelsPerMeter, position.y * pixelsPerMeter);
                button->setPosition(screenPos);
                button->setGeometry(screenPos.x(), screenPos.y(), GATE_SIZE, GATE_SIZE);
            }
        }
    };

    updateButtons(andGates);
    updateButtons(orGates);
    updateButtons(inverters);
}
