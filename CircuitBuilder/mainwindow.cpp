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
    , ui(new Ui::MainWindow), circuit{Circuit(parent)}, draggableButtons{}
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
    group->addAction(clear);

    connect(&circuit, &Circuit::nodeDeleted, this, &MainWindow::handleNodeDeleted);


    connect(ui->actionAndGate, &QAction::triggered, this, &MainWindow::onAndGateClicked);
    connect(ui->actionOrGate, &QAction::triggered, this, &MainWindow::onOrGateClicked);
    connect(ui->actionInverter, &QAction::triggered, this, &MainWindow::onInverterClicked);
    connect(ui->actionNandGate, &QAction::triggered, this, &MainWindow::onNandGateClicked);
    connect(ui->actionNorGate, &QAction::triggered, this, &MainWindow::onNorGateClicked);
    connect(ui->actionXorGate, &QAction::triggered, this, &MainWindow::onXorGateClicked);
    connect(ui->actionXnorGate, &QAction::triggered, this, &MainWindow::onXnorGateClicked);

    connect(ui->actionWire, &QAction::triggered, this, &MainWindow::onWireClicked);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteClicked);

    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onClearClicked);

    connect(this, &MainWindow::addButtonToCircuit, &circuit, &Circuit::addButton);

    connect(&circuit, &Circuit::allConnections, this, &MainWindow::drawWire);

    //physics set up
    initializePhysics();

    // Setup physics update timer
    physicsTimer = new QTimer(this);
    connect(physicsTimer, &QTimer::timeout, this, &MainWindow::updatePhysics);
    physicsTimer->start(16); // 60 FPS approximately

    //background label
    backgroundGridLabel = new QLabel(ui->centralwidget);
    backgroundGridLabel->setGeometry(0, 0, 600, 600);
    backgroundGridLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    // // Create  pixmap
    backgroundPixmap = new QPixmap(backgroundGridLabel->size());
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
    //andGates.push_back(createGateButton(GateType::AND_GATE, ui->actionAndGate->icon()));
    draggableButtons.push_back(createGateButton(GateType::AND_GATE, ui->actionAndGate->icon()));
}
void MainWindow::onOrGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::OR_GATE, ui->actionOrGate->icon()));
}
void MainWindow::onInverterClicked(){
    draggableButtons.push_back(createGateButton(GateType::INVERTER, ui->actionInverter->icon()));

}
void MainWindow::onNandGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::NAND_GATE, ui->actionNandGate->icon()));
}
void MainWindow::onNorGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::NOR_GATE, ui->actionNorGate->icon()));
}
void MainWindow::onXorGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::XOR_GATE, ui->actionXorGate->icon()));
}
void MainWindow::onXnorGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::XNOR_GATE, ui->actionXnorGate->icon()));
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

    updateButtons(draggableButtons);

    //draw wires
    if((int)draggableButtons.size() >1 ){
       // FIXME drawWire(draggableButtons.at(0),draggableButtons.at(1));
    }
}

void MainWindow::onClearClicked()
{
    // Update all buttons gravity
    auto updateButtons = [](const vector<DraggableButton*>& buttons) {
        for (auto button : buttons) {

                // Convert physics coordinates to screen coordinates

            b2Body* currentBody = button->getPhysicsBody();
            b2Vec2 vectr(0.0f, 9.8f);
            currentBody->SetLinearVelocity(vectr);

        }
    };

    updateButtons(draggableButtons);

}

DraggableButton* MainWindow::createGateButton(const GateType gateType, const QIcon& icon)
{
    // Gate gate(gateType);
    // DraggableButton* newButton = new DraggableButton(gateType, this, &gate);

    Gate* gate = new Gate(gateType);
    DraggableButton* newButton = new DraggableButton(gateType, this, gate);

    emit addButtonToCircuit(newButton, gateType);

    QPoint globalMousePos = QCursor::pos();
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
    newButton->setPosition(widgetPos);
    newButton->setGeometry(widgetPos.x(), widgetPos.y(), GATE_SIZE, GATE_SIZE);
    newButton->show();
    createPhysicsBody(newButton);

    newButton->setIconSize(QSize(GATE_SIZE, GATE_SIZE));
    newButton->setIcon(icon);

    connect(ui->actionWire, &QAction::triggered, newButton, &DraggableButton::setWireMode);

    connect(ui->actionDelete, &QAction::triggered, newButton, &DraggableButton::setDeleteMode);
    //connect(newButton, &DraggableButton::sendTwoButtons, this, &MainWindow::drawWire);  FIXME new signal use and signature

    return newButton;
}

void MainWindow::drawWire(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections){

    QPainter painter(backgroundPixmap);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));

    backgroundPixmap->fill(Qt::transparent);

    // Loop through each starting button and its wires
    for (DraggableButton* button1 : connections.keys())
    {
        QPoint startPos = button1->getPosition() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);

        // Loop over each connection for button1
        for (const QPair<DraggableButton*, int>& connection : connections.value(button1))
        {
            DraggableButton* button2 = connection.first;
            int input = connection.second;
            QPoint endPos = button2->getPosition() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);

            // Adjust the end position
            QPoint offset;
            switch (input) {
            case 1: offset = QPoint(-10, -12); break;
            case 2: offset = QPoint(-10, 12); break;
            default: offset = QPoint(0, 0); break;
            }
            endPos += offset;

            // Calculate mid-point for routing the wire
            int midX = (startPos.x() + endPos.x()) / 2;
            QPoint p1(midX, startPos.y());
            QPoint p2(midX, endPos.y());

            // Draw three segments
            painter.drawLine(startPos, p1);
            painter.drawLine(p1, p2);
            painter.drawLine(p2, endPos);
        }
    }

    backgroundGridLabel->setPixmap(*backgroundPixmap);
}

void MainWindow::onWireClicked(bool checked) {
    if (checked) {
        ui->actionAndGate->setEnabled(false);
        ui->actionOrGate->setEnabled(false);
        ui->actionNorGate->setEnabled(false);
        ui->actionXorGate->setEnabled(false);
        ui->actionXnorGate->setEnabled(false);
        ui->actionNandGate->setEnabled(false);
        ui->actionInverter->setEnabled(false);
    }
    else {
        ui->actionAndGate->setEnabled(true);
        ui->actionOrGate->setEnabled(true);
        ui->actionNorGate->setEnabled(true);
        ui->actionXorGate->setEnabled(true);
        ui->actionXnorGate->setEnabled(true);
        ui->actionNandGate->setEnabled(true);
        ui->actionInverter->setEnabled(true);
    }
}

void MainWindow::onDeleteClicked(bool checked){
    if (checked){
        ui->actionAndGate->setEnabled(false);
        ui->actionOrGate->setEnabled(false);
        ui->actionNorGate->setEnabled(false);
        ui->actionXorGate->setEnabled(false);
        ui->actionXnorGate->setEnabled(false);
        ui->actionNandGate->setEnabled(false);
        ui->actionInverter->setEnabled(false);
        ui->actionWire->setEnabled(false);
    }

    else {
        ui->actionAndGate->setEnabled(true);
        ui->actionOrGate->setEnabled(true);
        ui->actionNorGate->setEnabled(true);
        ui->actionXorGate->setEnabled(true);
        ui->actionXnorGate->setEnabled(true);
        ui->actionNandGate->setEnabled(true);
        ui->actionInverter->setEnabled(true);
        ui->actionWire->setEnabled(true);

    }
}


void MainWindow::handleNodeDeleted(DraggableButton* button) {
    // Remove from vector
    draggableButtons.erase(
        std::remove(draggableButtons.begin(), draggableButtons.end(), button),
        draggableButtons.end()
        );
}
