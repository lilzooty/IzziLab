#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <QPainter>
#include <QActionGroup>
#include <QPushButton>
#include "draggableGate.h"
#include <QMessageBox>
#include <qtoolbutton.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), circuit{Circuit(parent)}, draggableGates{}
{

    ui->setupUi(this);

    //

    // Get screen geometry
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // Calculate 80% size
    int width = screenGeometry.width() * 0.9;
    int height = screenGeometry.height() * 0.9;

    // Resize and center
    resize(width, height);

    // Center on screen
    int x = (screenGeometry.width() - width) / 2 + screenGeometry.x();
    int y = (screenGeometry.height() - height) / 2 + screenGeometry.y();
    move(x, y);

    //create pointers for toolbar buttons except delete gate and add wire
    QAction* andGate = ui->actionAndGate;
    QAction* orGate = ui->actionOrGate;
    QAction* inverter = ui->actionInverter;
    QAction* clear = ui->actionClear;
    QAction* nandGate = ui->actionNandGate;
    QAction* norGate = ui->actionNorGate;
    QAction* xorGate = ui->actionXorGate;
    QAction* inputGate = ui->actionInputGate;
    QAction* outputGate = ui->actionOutputGate;

    QActionGroup *group = new QActionGroup(this);

    group->addAction(andGate);
    group->addAction(orGate);
    group->addAction(nandGate);
    group->addAction(norGate);
    group->addAction(xorGate);
    group->addAction(inverter);
    group->addAction(clear);
    group->addAction(inputGate);
    group->addAction(outputGate);

    disableToolBarActions();

    connect(&circuit, &Circuit::nodeDeleted, this, &MainWindow::handleNodeDeleted);


    connect(ui->actionAndGate, &QAction::triggered, this, &MainWindow::onAndGateClicked);
    connect(ui->actionOrGate, &QAction::triggered, this, &MainWindow::onOrGateClicked);
    connect(ui->actionInverter, &QAction::triggered, this, &MainWindow::onInverterClicked);
    connect(ui->actionNandGate, &QAction::triggered, this, &MainWindow::onNandGateClicked);
    connect(ui->actionNorGate, &QAction::triggered, this, &MainWindow::onNorGateClicked);
    connect(ui->actionXorGate, &QAction::triggered, this, &MainWindow::onXorGateClicked);
    connect(ui->actionInputGate, &QAction::triggered, this, &MainWindow::onInputGateClicked);
    connect(ui->actionOutputGate, &QAction::triggered, this, &MainWindow::onOutputGateClicked);
    connect(&circuit, &Circuit::evaluationAnimation, this, &MainWindow::evaluationAnimation);

    connect(ui->actionWire, &QAction::triggered, this, &MainWindow::onWireClicked);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteClicked);

    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onClearClicked);
    connect(this, &MainWindow::clearCircuit, &circuit, &Circuit::onClear);

    connect(this, &MainWindow::addGateToCircuit, &circuit, &Circuit::addButton);

    connect(&circuit, &Circuit::allConnections, this, &MainWindow::drawWire);

    // connect(&circuit, &Circuit::sendEvaluation, this, &MainWindow::getNextLevel);
    // connect(this, &MainWindow::nextLevel, &circuit, &Circuit::levelUp);
    // connect(&circuit, &Circuit::sendLevel, this, &MainWindow::drawNewLevel);
    // connect(&circuit, &Circuit::sendDescription, this, &MainWindow::displayLevelDescription);
    // connect(&circuit, &Circuit::endGame, this, &MainWindow::gameOver);



    // Physics set up
    initializePhysics();

    // Setup physics update timer
    physicsTimer = new QTimer(this);
    connect(physicsTimer, &QTimer::timeout, this, &MainWindow::updatePhysics);
    physicsTimer->start(16); // 60 FPS approximately

    // Background label
    backgroundGridLabel = new QLabel(ui->centralwidget);
    backgroundGridLabel->setGeometry(0, 0, this->width(), this->height());
    backgroundGridLabel->setAttribute(Qt::WA_TransparentForMouseEvents);


    // Create  pixmap
    backgroundPixmap = new QPixmap();
    backgroundGridLabel->setPixmap(*backgroundPixmap);
    backgroundGridLabel->setScaledContents(true);

    // Bring others forward IF BUTTONS ARE NOT WORKING THIS COULD BE WHY
    ui->welcomeText->raise();
    ui->startButton->raise();
    ui->projectsList->raise();
    ui->logoLabel->raise();

    // ui->gridLayoutWidget->raise();


    // for (int level = 1; level <= 12; level++) {
    //     QString buttonName = QString("ButtonLevel%1").arg(level);
    //     QPushButton* button = findChild<QPushButton*>(buttonName);
    //     if (button) {
    //         button->setStyleSheet(levelButtonStyle);
    //         connect(button, &QPushButton::clicked, this, [this, level]() {
    //             startLevel(level);
    //         });
    //     }
    // }
}

MainWindow::~MainWindow() {
    if(physicsWorld){
        delete physicsWorld;
    }
    delete ui;
}






void MainWindow::onAndGateClicked(){
    draggableGates.push_back(createGateButton(GateType::AND_GATE, ui->actionAndGate->icon()));
    qDebug() << "added AND Gate. There are: " << draggableGates.size() << "total gates in circuit";
}
void MainWindow::onOrGateClicked(){
    draggableGates.push_back(createGateButton(GateType::OR_GATE, ui->actionOrGate->icon()));
    qDebug() << "added OR Gate. There are: " << draggableGates.size() << "total gates in circuit";

}
void MainWindow::onInverterClicked(){
    draggableGates.push_back(createGateButton(GateType::INVERTER, ui->actionInverter->icon()));
    qDebug() << "added NOT Gate. There are: " << draggableGates.size() << "total gates in circuit";


}
void MainWindow::onNandGateClicked(){
    draggableGates.push_back(createGateButton(GateType::NAND_GATE, ui->actionNandGate->icon()));
    qDebug() << "added NAND Gate. There are: " << draggableGates.size() << "total gates in circuit";

}
void MainWindow::onNorGateClicked(){
    draggableGates.push_back(createGateButton(GateType::NOR_GATE, ui->actionNorGate->icon()));
    qDebug() << "added NOR Gate. There are: " << draggableGates.size() << "total gates in circuit";

}
void MainWindow::onXorGateClicked(){
    draggableGates.push_back(createGateButton(GateType::XOR_GATE, ui->actionXorGate->icon()));
    qDebug() << "added XOR Gate. There are: " << draggableGates.size() << "total gates in circuit";

}

void MainWindow::onInputGateClicked(){
    draggableGates.push_back(createGateButton(GateType::INPUT, ui->actionInputGate->icon()));
    qDebug() << "added IN Gate. There are: " << draggableGates.size() << "total gates in circuit";
}


void MainWindow::onOutputGateClicked(){
    draggableGates.push_back(createGateButton(GateType::OUTPUT, ui->actionOutputGate->icon()));
    qDebug() << "added OUT Gate. There are: " << draggableGates.size() << "total gates in circuit";

}

void MainWindow::on_startButton_clicked()
{
    startGame();
}

void MainWindow::startGame() {
    enableToolBarActions();
    // fix zoom
    backgroundPixmap = new QPixmap(backgroundGridLabel->size());
    backgroundGridLabel->setScaledContents(false);

    // hide home/main view UI components
    ui->startButton->hide();
    ui->logoLabel->hide();
    ui->welcomeText->hide();
    ui->projectsList->hide();
}

void MainWindow::createPhysicsBody(draggableGate* button) {
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

void MainWindow::initializePhysics() {
    physicsWorld = new b2World(b2Vec2(.0f, .0f));
}

void MainWindow::updatePhysics() {
    // Step the physics simulation
    physicsWorld->Step(1.0f/60.0f, 8, 3);  // Increase iteration counts

    // Update all buttons positions
    auto updateButtons = [this](const vector<draggableGate*>& buttons) {
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

    updateButtons(draggableGates);
}

void MainWindow::onClearClicked() {
    // Update all buttons gravity
    // for (auto button : draggableGates) {
    //     button->buttonDelete();
    // }
    // QTimer::singleShot(3000, this, [this]() {draggableGates.clear();});
    draggableGates.clear();

    // eventually lets make the circuit not a part of the mainwindow class itself, rather for an 'editorview'
    emit clearCircuit();

    qDebug() << "buttons deleted. There are now " << draggableGates.size() << "buttons left in the cirucuit";
}

draggableGate* MainWindow::createGateButton(const GateType gateType, const QIcon& icon) {
    Gate* gate = new Gate(gateType);
    draggableGate* newButton = new draggableGate(gateType, this, gate);

    emit addGateToCircuit(newButton, gateType);

    QPoint globalMousePos = QCursor::pos() + QPoint(25,-17);
    QPoint widgetPos = this->mapFromGlobal(globalMousePos);
    newButton->setPosition(widgetPos);
    newButton->setGeometry(widgetPos.x(), widgetPos.y(), GATE_SIZE, GATE_SIZE);
    newButton->show();
    createPhysicsBody(newButton);

    newButton->setIconSize(QSize(GATE_SIZE, GATE_SIZE));
    newButton->setIcon(icon);

    connect(ui->actionWire, &QAction::triggered, newButton, &draggableGate::setWireMode);
    connect(ui->actionDelete, &QAction::triggered, newButton, &draggableGate::setDeleteMode);

    return newButton;
}

void MainWindow::drawWire(QMap<draggableGate*, QVector<QPair<draggableGate*, int>>> connections) {
    backgroundPixmap->fill(Qt::transparent);

    // Loop through each starting button and its wires
    for (draggableGate* sourceButton : connections.keys()) {
        // QPoint startPos = sourceButton->pos();
        QPoint startPos = sourceButton->pos();
        QPoint startOffset = QPoint(GATE_SIZE / 2, GATE_SIZE / 2);
        startPos += startOffset;

        // Loop over each connection for source button
        for (const auto& connection : connections.value(sourceButton)) {
            draggableGate* targetButton = connection.first;
            int inputPort = connection.second;

            // Calculate end position with input port offset
            QPoint endPos = targetButton->pos();
            QPoint offset;
            switch (inputPort) {
            case 1: offset = QPoint(-GATE_SIZE/2, GATE_SIZE/2); break;
            case 2: offset = QPoint(0, GATE_SIZE * 0.65); break;
            default: offset = QPoint(-GATE_SIZE/2, GATE_SIZE/2); break;
            }
            endPos += offset;

            // Handle backward wiring (when start is to the right of end)
            if (startPos.x() > endPos.x()) {
                int verticalOffset = GATE_SIZE;
                // See if it closer to go up or down
                if (startPos.y() + GATE_SIZE < endPos.y()) {
                    verticalOffset = -verticalOffset;
                }

                // Draw first segment
                QPoint p1(startPos.x() + GATE_SIZE, startPos.y());
                drawWireArrow(startPos, p1, false);

                // Draw second segment (vertical)
                QPoint p2(p1.x(), p1.y() - verticalOffset);
                drawWireArrow(p1, p2, false);

                // Draw third and fourth segments
                QPoint p3(endPos.x() - GATE_SIZE, endPos.y());
                QPoint p4(p3.x(), p3.y() - verticalOffset);
                drawWireArrow(p4, p3, true);
                drawWireArrow(p3, endPos, true);

                // Updated position to draw connecting 3 wires
                endPos = p4;
                startPos = p2;
            }

                int midX = (startPos.x() + endPos.x()) / 2;
                QPoint p1(midX, startPos.y());
                QPoint p2(midX, endPos.y());

                drawWireArrow(startPos, p1, true);
                drawWireArrow(p1, p2, true);
                drawWireArrow(p2, endPos, true);

        }
    }

    backgroundGridLabel->setPixmap(*backgroundPixmap);
}

void MainWindow::drawWireArrow(QPoint start,  QPoint end, bool animating) {
    int arrowSize = 10;

    QPainter painter(backgroundPixmap);

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
    if(animating){
        painter.setPen(QPen(Qt::blue, 4, Qt::SolidLine, Qt::RoundCap));
    }

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

    painter.setBrush(QBrush(Qt::blue));
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


void MainWindow::handleNodeDeleted(draggableGate* button) {
    // Remove from vector
    draggableGates.erase(
        std::remove(draggableGates.begin(), draggableGates.end(), button),
        draggableGates.end()
    );
}



void MainWindow::enableToolBarActions() {
    ui->actionAndGate->setEnabled(true);
    ui->actionOrGate->setEnabled(true);
    ui->actionNorGate->setEnabled(true);
    ui->actionXorGate->setEnabled(true);
    ui->actionNandGate->setEnabled(true);
    ui->actionInverter->setEnabled(true);
    ui->actionWire->setEnabled(true);
    ui->actionDelete->setEnabled(true);
    ui->actionClear->setEnabled(true);
    ui->actionInputGate->setEnabled(true);
    ui->actionOutputGate->setEnabled(true);

}

void MainWindow::disableToolBarActions() {
    ui->actionAndGate->setEnabled(false);
    ui->actionOrGate->setEnabled(false);
    ui->actionNorGate->setEnabled(false);
    ui->actionXorGate->setEnabled(false);
    ui->actionNandGate->setEnabled(false);
    ui->actionInverter->setEnabled(false);
    ui->actionWire->setEnabled(false);
    ui->actionDelete->setEnabled(false);
    ui->actionClear->setEnabled(false);
    ui->actionInputGate->setEnabled(false);
    ui->actionOutputGate->setEnabled(false);

}


void MainWindow::evaluationAnimation(QMap<draggableGate*, QVector<QPair<draggableGate*, int>>> connections) {
    QVector<QPair<QPoint,QPoint>> wireSegments;

    // Collect all wire segments
    for (draggableGate* sourceButton : connections.keys()) {
        QPoint startPos = sourceButton->pos() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);

        for (const auto& connection : connections.value(sourceButton)) {
            draggableGate* targetButton = connection.first;
            int inputPort = connection.second;

            QPoint endPos = targetButton->pos() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);
            QPoint offset;
            switch (inputPort) {
            case 1: offset = QPoint(-10, -12); break;
            case 2: offset = QPoint(-10, 12); break;
            default: offset = QPoint(0, 0); break;
            }
            endPos += offset;

            if (startPos.x() > endPos.x()) {
                int verticalOffset = GATE_SIZE;
                if (startPos.y() + GATE_SIZE < endPos.y()) {
                    verticalOffset = -verticalOffset;
                }

                // Store backward wiring segments
                QPoint p1(startPos.x() + GATE_SIZE, startPos.y());
                wireSegments.push_back(QPair<QPoint,QPoint>(startPos, p1));

                QPoint p2(p1.x(), p1.y() - verticalOffset);
                wireSegments.push_back(QPair<QPoint,QPoint>(p1, p2));

                QPoint p3(endPos.x() - GATE_SIZE, endPos.y());
                QPoint p4(p3.x(), p3.y() - verticalOffset);
                wireSegments.push_back(QPair<QPoint,QPoint>(p4, p3));
                wireSegments.push_back(QPair<QPoint,QPoint>(p3, endPos));

                endPos = p4;
                startPos = p2;
            }

            // Store forward wiring segments
            int midX = (startPos.x() + endPos.x()) / 2;
            QPoint p1(midX, startPos.y());
            QPoint p2(midX, endPos.y());

            wireSegments.push_back(QPair<QPoint,QPoint>(startPos, p1));
            wireSegments.push_back(QPair<QPoint,QPoint>(p1, p2));
            wireSegments.push_back(QPair<QPoint,QPoint>(p2, endPos));
        }
    }

    std::sort(wireSegments.begin(), wireSegments.end(), []( QPair<QPoint, QPoint>& p1,  QPair<QPoint, QPoint>& p2) {
        return p1.first.x() < p2.first.x();
    });

    // Draw wire segments with delay
    QTimer* animationTimer = new QTimer(this);
    int* currentSegment = new int(0);

    connect(animationTimer, &QTimer::timeout, this, [this, wireSegments, currentSegment, animationTimer, connections]() {
        if (*currentSegment < wireSegments.size()) {
            drawWireArrow(wireSegments[*currentSegment].first, wireSegments[*currentSegment].second, true);

            backgroundGridLabel->setPixmap(*backgroundPixmap);

            (*currentSegment)++;
        } else {
            // Animation complete
            animationTimer->stop();
            delete currentSegment;
            animationTimer->deleteLater();

        }
    });
    animationTimer->start(400);
}







void MainWindow::returnToMenu(){
    //clear gates on screen
    onClearClicked();
    //disable toolbar
    disableToolBarActions();

    // Create  pixmap
    backgroundPixmap = new QPixmap();
    backgroundGridLabel->setPixmap(*backgroundPixmap);
    backgroundGridLabel->setScaledContents(true);

    // show home/main view UI components
    ui->welcomeText->show();
    ui->startButton->show();
    ui->projectsList->show();
    ui->logoLabel->show();
}

void MainWindow::on_actionMENU_triggered()
{
    returnToMenu();
}





