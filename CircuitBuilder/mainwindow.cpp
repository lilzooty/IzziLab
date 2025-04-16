#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <QPainter>
#include <QActionGroup>
#include <QPushButton>
#include "draggablebutton.h"
#include "astarpathfinder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), andGates{}, orGates{}, inverters{}, circuit{Circuit(parent)}
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

    QRect rect(400, 400, 250, 250);

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

    connect(this, &MainWindow::addButtonToCircuit, &circuit, &Circuit::addButton);
    connect(this, &MainWindow::addGate, &circuit, &Circuit::registerNode);


    // DraggableButton* button = new DraggableButton(this);  // or however you set it up
    // connect(ui->actionWire, &QAction::triggered, button, &DraggableButton::wireMode);

    // connect(this, &MainWindow::addNode, circuit, &Circuit::addNode);

    // // connect dragbut to circuit
    // for (DraggableButton* btn : draggableButtons) {
    //     connect(ui->actionWire, &QAction::triggered, btn, &DraggableButton::setWireMode);
    // }

    //physics set up
    // Initialize physics
    initializePhysics();

    // Setup physics update timer
    physicsTimer = new QTimer(this);
    connect(physicsTimer, &QTimer::timeout, this, &MainWindow::updatePhysics);
    physicsTimer->start(16); // 60 FPS approximately

    //background label
    backgroundGridLabel = new QLabel(ui->centralwidget);
    backgroundGridLabel->setGeometry(0, 0, 600, 600);
    backgroundGridLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Create the pixmap
    QPixmap* backgroundPixmap = new QPixmap(backgroundGridLabel->size());
    backgroundPixmap->fill(Qt::transparent);
    backgroundGridLabel->setPixmap(*backgroundPixmap);
    backgroundGridLabel->show();

}

MainWindow::~MainWindow()
{
    if(physicsWorld){
        delete physicsWorld;
    }
    delete ui;
}

void MainWindow::onAndGateClicked(){
    andGates.push_back(createGateButton(GateType::AND_GATE, ui->actionAndGate->icon()));
    // emit addNode(GateType::AND_GATE);
}
void MainWindow::onOrGateClicked(){
    orGates.push_back(createGateButton(GateType::OR_GATE, ui->actionOrGate->icon()));
    // emit addNode(GateType::OR_GATE);
}
void MainWindow::onInverterClicked(){
    inverters.push_back(createGateButton(GateType::INVERTER, ui->actionInverter->icon()));
    // emit addNode(GateType::INVERTER);

}
void MainWindow::onNandGateClicked(){
    nandGates.push_back(createGateButton(GateType::NAND_GATE, ui->actionNandGate->icon()));
    // emit addNode(GateType::NAND_GATE);
}
void MainWindow::onNorGateClicked(){
    norGates.push_back(createGateButton(GateType::NOR_GATE, ui->actionNorGate->icon()));
    // emit addNode(GateType::NOR_GATE);
}
void MainWindow::onXorGateClicked(){
    xnorGates.push_back(createGateButton(GateType::XOR_GATE, ui->actionXorGate->icon()));
    // emit addNode(GateType::XOR_GATE);
}
void MainWindow::onXnorGateClicked(){
    xnorGates.push_back(createGateButton(GateType::XNOR_GATE, ui->actionXnorGate->icon()));
    //emit addNode(GateType::XNOR_GATE);
}

void MainWindow::onWireClicked()
{
    // emit wireMode();
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

DraggableButton* MainWindow::createGateButton(const GateType gateType, const QIcon& icon)
{
    Gate gate(gateType);
    DraggableButton* newButton = new DraggableButton(gateType, this, &gate);

    emit addGate(&gate);

    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
    newButton->setPosition(widgetPos);
    newButton->setGeometry(widgetPos.x(), widgetPos.y(), GATE_SIZE, GATE_SIZE);
    newButton->show();
    createPhysicsBody(newButton);

    newButton->setIconSize(QSize(GATE_SIZE, GATE_SIZE));
    newButton->setIcon(icon);

    connect(ui->actionWire, &QAction::triggered, newButton, &DraggableButton::setWireMode);
    connect(newButton, &DraggableButton::sendTwoButtons, this, &MainWindow::drawWire);

    emit addButtonToCircuit(newButton);

    return newButton;
}



void MainWindow::drawWire(DraggableButton *button1, DraggableButton *button2) {

    //in .h file
    //QLabel* backgroundGridLabel;

    // backgroundGridLabel = new QLabel(ui->centralwidget);
    // backgroundGridLabel->setGeometry(0, 0, 600, 600);
    // backgroundGridLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    // // Create the pixmap
    // QPixmap* backgroundPixmap = new QPixmap(backgroundGridLabel->size());
    // backgroundPixmap->fill(Qt::transparent);
    // backgroundGridLabel->setPixmap(*backgroundPixmap);
    // backgroundGridLabel->show();

    QPixmap pixmap(backgroundGridLabel->size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));

    QPoint startPos = button1->getPosition() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);
    QPoint endPos = button2->getPosition() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);

    // Draw a nice routing path using multiple lines
    int midX = (startPos.x() + endPos.x()) / 2;

    QPoint p1(midX, startPos.y());
    QPoint p2(midX, endPos.y());

    // Draw three segments for better routing
    painter.drawLine(startPos, p1);
    painter.drawLine(p1, p2);
    painter.drawLine(p2, endPos);
    backgroundGridLabel->setPixmap(pixmap);
}

