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
    QAction* inputGate = ui->actionInputGate;

    QActionGroup *group = new QActionGroup(this);

    group->addAction(andGate);
    group->addAction(orGate);
    group->addAction(nandGate);
    group->addAction(norGate);
    group->addAction(xorGate);
    group->addAction(xnorGate);
    group->addAction(inverter);
    group->addAction(clear);
    group->addAction(inputGate);

    disableToolBarActions();

    connect(&circuit, &Circuit::nodeDeleted, this, &MainWindow::handleNodeDeleted);

    connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::startGame);


    connect(ui->EvaluateButton, &QPushButton::pressed, &circuit, &Circuit::onEvaluate);

    connect(ui->actionAndGate, &QAction::triggered, this, &MainWindow::onAndGateClicked);
    connect(ui->actionOrGate, &QAction::triggered, this, &MainWindow::onOrGateClicked);
    connect(ui->actionInverter, &QAction::triggered, this, &MainWindow::onInverterClicked);
    connect(ui->actionNandGate, &QAction::triggered, this, &MainWindow::onNandGateClicked);
    connect(ui->actionNorGate, &QAction::triggered, this, &MainWindow::onNorGateClicked);
    connect(ui->actionXorGate, &QAction::triggered, this, &MainWindow::onXorGateClicked);
    connect(ui->actionXnorGate, &QAction::triggered, this, &MainWindow::onXnorGateClicked);
     connect(ui->actionInputGate, &QAction::triggered, this, &MainWindow::onInputGateClicked);

    connect(ui->actionWire, &QAction::triggered, this, &MainWindow::onWireClicked);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteClicked);

    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onClearClicked);

    connect(this, &MainWindow::addButtonToCircuit, &circuit, &Circuit::addButton);

    connect(&circuit, &Circuit::allConnections, this, &MainWindow::drawWire);

    connect(&circuit, &Circuit::sendEvaluation, this, &MainWindow::getNextLevel);
    connect(this, &MainWindow::nextLevel, &circuit, &Circuit::levelUp);
    connect(&circuit, &Circuit::sendLevel, this, &MainWindow::drawNewLevel);

    connect(ui->startButton, &QPushButton::pressed, &circuit, &Circuit::levelUp);

    //physics set up
    initializePhysics();

    // Setup physics update timer
    physicsTimer = new QTimer(this);
    connect(physicsTimer, &QTimer::timeout, this, &MainWindow::updatePhysics);
    physicsTimer->start(16); // 60 FPS approximately

    //background label
    backgroundGridLabel = new QLabel(ui->centralwidget);
    backgroundGridLabel->setGeometry(0, 0, 800, 600);
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
void MainWindow::onInputGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::INPUT, ui->actionInputGate->icon()));
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

    return newButton;


}

// DraggableButton* MainWindow::createGateButton(const GateType gateType, const QIcon& icon, QPoint pos)
// {
//     Gate* gate = new Gate(gateType);
//     DraggableButton* newButton = new DraggableButton(gateType, this, gate, pos);
//     newButton->setPosition(pos);

//     qDebug()<< newButton->pos();
//     qDebug()<<pos;

//     emit addButtonToCircuit(newButton, gateType);

//     QPoint globalMousePos = QCursor::pos();
//     QPoint widgetPos = this->mapFromGlobal(globalMousePos);
//     newButton->setPosition(widgetPos);
//     newButton->setGeometry(widgetPos.x(), widgetPos.y(), GATE_SIZE, GATE_SIZE);
//     newButton->show();
//     createPhysicsBody(newButton);

//     newButton->setIconSize(QSize(GATE_SIZE, GATE_SIZE));
//     newButton->setIcon(icon);

//     connect(ui->actionWire, &QAction::triggered, newButton, &DraggableButton::setWireMode);
//     connect(ui->actionDelete, &QAction::triggered, newButton, &DraggableButton::setDeleteMode);

//     return newButton;


// }

void MainWindow::drawWire(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections){

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



            // draw arrows
            drawWireArrow(startPos, p1);
            drawWireArrow( p1, p2);
            drawWireArrow( p2, endPos);

        }
    }

    backgroundGridLabel->setPixmap(*backgroundPixmap);
}

void MainWindow::drawWireArrow(QPoint start,  QPoint end) {

    int arrowSize = 10;

    QPainter painter(backgroundPixmap);

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(start,end);

    QPoint diff = end - start;
    QPoint arrowPos = (start + end) / 2;  // Place arrow in middle of segment

    QPolygon triangle;
    if (abs(diff.x()) > abs(diff.y())) {  // Horizontal line
        if (diff.x() > 0) {  // Going right
            triangle << arrowPos
                     << QPoint(arrowPos.x() - arrowSize, arrowPos.y() - arrowSize/2)
                     << QPoint(arrowPos.x() - arrowSize, arrowPos.y() + arrowSize/2);
        } else {  // Going left
            triangle << arrowPos
                     << QPoint(arrowPos.x() + arrowSize, arrowPos.y() - arrowSize/2)
                     << QPoint(arrowPos.x() + arrowSize, arrowPos.y() + arrowSize/2);
        }
    } else {  // Vertical line
        if (diff.y() > 0) {  // Going down
            triangle << arrowPos
                     << QPoint(arrowPos.x() - arrowSize/2, arrowPos.y() - arrowSize)
                     << QPoint(arrowPos.x() + arrowSize/2, arrowPos.y() - arrowSize);
        } else {  // Going up
            triangle << arrowPos
                     << QPoint(arrowPos.x() - arrowSize/2, arrowPos.y() + arrowSize)
                     << QPoint(arrowPos.x() + arrowSize/2, arrowPos.y() + arrowSize);
        }
    }

    painter.setBrush(QBrush(Qt::white));
    painter.drawPolygon(triangle);
}


void MainWindow::onWireClicked(bool checked) {
    if (checked) {
        disableToolBarActions();
        ui->actionWire->setEnabled(true);
        ui->actionClear->setEnabled(true);
    }
    else {
        enableToolBarActions();
    }
}

void MainWindow::onDeleteClicked(bool checked){
    if (checked){
        disableToolBarActions();
        ui->actionDelete->setEnabled(true);
        ui->actionClear->setEnabled(true);
    }

    else {
        enableToolBarActions();
    }
}


void MainWindow::handleNodeDeleted(DraggableButton* button) {
    // Remove from vector
    draggableButtons.erase(
        std::remove(draggableButtons.begin(), draggableButtons.end(), button),
        draggableButtons.end()
        );
}

void MainWindow::startGame() {
    enableToolBarActions();

    ui->startButton->hide();
    ui->startButton->deleteLater();

    ui->textEdit->hide();
    ui->textEdit->deleteLater();
}

void MainWindow::enableToolBarActions() {
    ui->actionAndGate->setEnabled(true);
    ui->actionOrGate->setEnabled(true);
    ui->actionNorGate->setEnabled(true);
    ui->actionXorGate->setEnabled(true);
    ui->actionXnorGate->setEnabled(true);
    ui->actionNandGate->setEnabled(true);
    ui->actionInverter->setEnabled(true);
    ui->actionWire->setEnabled(true);
    ui->actionDelete->setEnabled(true);
    ui->actionClear->setEnabled(true);
    ui->actionInputGate->setEnabled(true);
}

void MainWindow::disableToolBarActions() {
    ui->actionAndGate->setEnabled(false);
    ui->actionOrGate->setEnabled(false);
    ui->actionNorGate->setEnabled(false);
    ui->actionXorGate->setEnabled(false);
    ui->actionXnorGate->setEnabled(false);
    ui->actionNandGate->setEnabled(false);
    ui->actionInverter->setEnabled(false);
    ui->actionWire->setEnabled(false);
    ui->actionDelete->setEnabled(false);
    ui->actionClear->setEnabled(false);
    ui->actionInputGate->setEnabled(false);
}

void MainWindow::drawNewLevel(int inputs, TruthTable* newTable){
    QPoint p;

    // Clearing the input and output layouts upon level instantiation.
    QLayoutItem *item;
    while(ui->inputLayout->count() > 0){
        QLayoutItem *item = ui->inputLayout->takeAt(0);
        QWidget* widget = item->widget();
        if(widget)
            delete widget;
        delete item;
    }

    while(ui->outputLayout->count() > 0){
        QLayoutItem *item = ui->outputLayout->takeAt(0);
        QWidget* widget = item->widget();
        if(widget)
            delete widget;
        delete item;
    }

    // Adding the new inputs and outputs
    for (int i = 0; i < inputs; i++){
        // CHANGE ICON OR ELSE
        p = QPoint(100, 100*i + 100);

        DraggableButton* input = createGateButton(GateType::INPUT, ui->actionInputGate->icon());

        ui->inputLayout->addWidget(input);

    }

    p = QPoint(600,300);
    DraggableButton* output = createGateButton(GateType::OUTPUT, ui->actionAndGate->icon());
    ui->outputLayout->addWidget(output);

    // pull data out of truthtable
    QTableWidget* tableWidget = ui->previewTableWidget;

    QList tableRows = newTable->getRows();

    int rowCount = tableRows.size();
    qDebug() << rowCount;
    int inputCount = tableRows[0].first.size();
    qDebug() << inputCount;
    int colCount = inputCount + 1;
    qDebug() << colCount;


    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(colCount);

    // Headers
    QStringList headers;
    for (int i = 0; i < inputCount; ++i)
        headers << QString("In%1").arg(i + 1);
    headers << "Out";

    tableWidget->setHorizontalHeaderLabels(headers);

    // Fill
    for (int row = 0; row < rowCount; ++row) {
        const QVector<int>& inputs = tableRows[row].first;
        int output = tableRows[row].second;

        for (int col = 0; col < inputCount; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(inputs[col]));
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, col, item);
        }

        QTableWidgetItem* outItem = new QTableWidgetItem(QString::number(output));
        outItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, inputCount, outItem);
    }
}

void MainWindow::getNextLevel(bool levelComplete){
    if (levelComplete){
        emit nextLevel();
    }
    else
    {
        // FIX ****************************
        emit nextLevel();
        // Display try again message.
    }

}
