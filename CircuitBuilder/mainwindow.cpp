#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <QActionGroup>
#include <QPushButton>
#include "draggablebutton.h"
#include "astarpathfinder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), andGates{}, orGates{}, inverters{}
{
    ui->setupUi(this);

    int tileSize = 50;

    int width = this->width();
    int height = this->height();

    int cols = width; // / 50;
    int rows = height; // / 50;

    grid.resize(rows, std::vector<bool>(cols, true));

    int startRow = 25;
    int endRow = 600;

    int startCol = 0;
    int endCol = 100;

    for(int row = startRow; row < endRow; row++) {
        for(int col = startCol; col < endCol; col++) {
            grid[row][col] = false;
        }
    }

    QAction* andGate = ui->actionAndGate;

    QAction* orGate = ui->actionOrGate;

    QAction* inverter = ui->actionInverter;

    QAction* wire = ui->actionWire;

    QAction* clear = ui->actionClear;

    QActionGroup *group = new QActionGroup(this);

    group->addAction(andGate);
    group->addAction(orGate);
    group->addAction(inverter);
    group->addAction(wire);
    group->addAction(clear);

    connect(ui->actionAndGate, &QAction::triggered, this, &MainWindow::onAndGateClicked);
    connect(ui->actionOrGate, &QAction::triggered, this, &MainWindow::onOrGateClicked);
    connect(ui->actionInverter, &QAction::triggered, this, &MainWindow::onInverterClicked);
    connect(ui->actionWire, &QAction::triggered, this, &MainWindow::onWireClicked);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onClearClicked);

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
    auto generateRandomVelocity = []() {
        float x = (std::rand() / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f;
        float y = (std::rand() / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f;

        x += (x == 0) * (x < 0 ? -1.0f : 1.0f);
        y += (y == 0) * (y < 0 ? -1.0f : 1.0f);

        return b2Vec2(x, y);
    };

    auto updateButtonVelocities = [&generateRandomVelocity](const vector<DraggableButton*>& buttons) {
        for (auto button : buttons) {
            button->getPhysicsBody()->SetLinearVelocity(generateRandomVelocity());
        }
    };

    updateButtonVelocities(andGates);
    updateButtonVelocities(orGates);
    updateButtonVelocities(inverters);

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

void MainWindow::onClearClicked()
{
    // Update all buttons gravity
    auto updateButtons = [this](const vector<DraggableButton*>& buttons) {
        for (auto button : buttons) {

                // Convert physics coordinates to screen coordinates

            b2Body* currentBody = button->getPhysicsBody();
                b2Vec2 vectr(0.0f, 9.8f);
            currentBody->SetLinearVelocity(vectr);



        }
    };
    updateButtons(andGates);
    updateButtons(orGates);
    updateButtons(inverters);
}

