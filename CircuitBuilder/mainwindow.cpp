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
    QAction* nandGate = ui->actionNandGate;
    QAction* norGate = ui->actionNorGate;
    QAction* xorGate = ui->actionXorGate;
    QAction* xnorGate = ui->actionXnorGate;

    QActionGroup *group = new QActionGroup(this);

    group->addAction(andGate);
    group->addAction(orGate);
    group->addAction(nandGate);
    group->addAction(norGate);
    group->addAction(xorGate);
    group->addAction(xnorGate);
    group->addAction(inverter);
    group->addAction(wire);
    group->addAction(clear);


    connect(ui->actionAndGate, &QAction::triggered, this, &MainWindow::onAndGateClicked);
    connect(ui->actionOrGate, &QAction::triggered, this, &MainWindow::onOrGateClicked);
    connect(ui->actionInverter, &QAction::triggered, this, &MainWindow::onInverterClicked);
    connect(ui->actionNandGate, &QAction::triggered, this, &MainWindow::onNandGateClicked);
    connect(ui->actionNorGate, &QAction::triggered, this, &MainWindow::onNorGateClicked);
    connect(ui->actionXorGate, &QAction::triggered, this, &MainWindow::onXorGateClicked);
    connect(ui->actionXnorGate, &QAction::triggered, this, &MainWindow::onXnorGateClicked);

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

void MainWindow::onAndGateClicked(){
    andGates.push_back(createGateButton("AND", ui->actionAndGate->icon()));
}
void MainWindow::onOrGateClicked(){
    orGates.push_back(createGateButton("OR", ui->actionOrGate->icon()));
}
void MainWindow::onInverterClicked(){
    inverters.push_back(createGateButton("NOT", ui->actionInverter->icon()));
}
void MainWindow::onNandGateClicked(){
    nandGates.push_back(createGateButton("NAND", ui->actionNandGate->icon()));
}
void MainWindow::onNorGateClicked(){
    norGates.push_back(createGateButton("NOR", ui->actionNorGate->icon()));
}
void MainWindow::onXorGateClicked(){
    xnorGates.push_back(createGateButton("XOR", ui->actionXorGate->icon()));
}
void MainWindow::onXnorGateClicked(){
    xnorGates.push_back(createGateButton("XNOR", ui->actionXnorGate->icon()));
}

void MainWindow::onWireClicked()
{
    // auto generateRandomVelocity = []() {
    //     float x = (std::rand() / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f;
    //     float y = (std::rand() / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f;

    //     x += (x == 0) * (x < 0 ? -1.0f : 1.0f);
    //     y += (y == 0) * (y < 0 ? -1.0f : 1.0f);

    //     return b2Vec2(x, y);
    // };

    // auto updateButtonVelocities = [&generateRandomVelocity](const vector<DraggableButton*>& buttons) {
    //     for (auto button : buttons) {
    //         button->getPhysicsBody()->SetLinearVelocity(generateRandomVelocity());
    //     }
    // };


    // updateButtonVelocities(andGates);
    // updateButtonVelocities(orGates);
    // updateButtonVelocities(inverters);
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
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0f;

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
    physicsWorld->Step(1.0f/60.0f, 8, 3);  // Increase iteration counts

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
    updateButtons(nandGates);
    updateButtons(norGates);
    updateButtons(xorGates);
    updateButtons(xnorGates);
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
    updateButtons(nandGates);
    updateButtons(norGates);
    updateButtons(xorGates);
    updateButtons(xnorGates);
}

DraggableButton* MainWindow::createGateButton(const QString& gateType, const QIcon& icon)
{
    DraggableButton* newButton = new DraggableButton(gateType, this);
    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
    newButton->setPosition(widgetPos);
    newButton->setGeometry(widgetPos.x(), widgetPos.y(), GATE_SIZE, GATE_SIZE);
    newButton->show();
    createPhysicsBody(newButton);

    newButton->setIconSize(QSize(GATE_SIZE, GATE_SIZE));
    newButton->setIcon(icon);

    return newButton;
}
