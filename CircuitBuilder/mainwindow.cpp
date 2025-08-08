#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <QPainter>
#include <QActionGroup>
#include <QPushButton>
#include "draggablebutton.h"
#include <QMessageBox>
#include <qtoolbutton.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), circuit{Circuit(parent)}, draggableButtons{}
{
    ui->setupUi(this);


    QAction* andGate = ui->actionAndGate;
    QAction* orGate = ui->actionOrGate;
    QAction* inverter = ui->actionInverter;
    QAction* clear = ui->actionClear;
    QAction* nandGate = ui->actionNandGate;
    QAction* norGate = ui->actionNorGate;
    QAction* xorGate = ui->actionXorGate;
    QAction* xnorGate = ui->actionXnorGate;
    QAction* inputGate = ui->actionInputGate;
    QAction* outputGate = ui->actionOutputGate;

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
    group->addAction(outputGate);

    disableToolBarActions();

    connect(&circuit, &Circuit::nodeDeleted, this, &MainWindow::handleNodeDeleted);

    connect(ui->EvaluateButton, &QPushButton::pressed, &circuit, &Circuit::onEvaluate);

    connect(ui->actionAndGate, &QAction::triggered, this, &MainWindow::onAndGateClicked);
    connect(ui->actionOrGate, &QAction::triggered, this, &MainWindow::onOrGateClicked);
    connect(ui->actionInverter, &QAction::triggered, this, &MainWindow::onInverterClicked);
    connect(ui->actionNandGate, &QAction::triggered, this, &MainWindow::onNandGateClicked);
    connect(ui->actionNorGate, &QAction::triggered, this, &MainWindow::onNorGateClicked);
    connect(ui->actionXorGate, &QAction::triggered, this, &MainWindow::onXorGateClicked);
    connect(ui->actionXnorGate, &QAction::triggered, this, &MainWindow::onXnorGateClicked);
    connect(ui->actionInputGate, &QAction::triggered, this, &MainWindow::onInputGateClicked);
    connect(&circuit, &Circuit::evaluationAnimation, this, &MainWindow::evaluationAnimation);

    connect(ui->actionWire, &QAction::triggered, this, &MainWindow::onWireClicked);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteClicked);

    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onClearClicked);
    connect(this, &MainWindow::clearCircuit, &circuit, &Circuit::onClear);

    connect(this, &MainWindow::addButtonToCircuit, &circuit, &Circuit::addButton);

    connect(&circuit, &Circuit::allConnections, this, &MainWindow::drawWire);

    // connect(&circuit, &Circuit::sendEvaluation, this, &MainWindow::getNextLevel);
    // connect(this, &MainWindow::nextLevel, &circuit, &Circuit::levelUp);
    // connect(&circuit, &Circuit::sendLevel, this, &MainWindow::drawNewLevel);
    // connect(&circuit, &Circuit::sendDescription, this, &MainWindow::displayLevelDescription);
    connect(&circuit, &Circuit::endGame, this, &MainWindow::gameOver);



    // Physics set up
    initializePhysics();

    // Setup physics update timer
    physicsTimer = new QTimer(this);
    connect(physicsTimer, &QTimer::timeout, this, &MainWindow::updatePhysics);
    physicsTimer->start(16); // 60 FPS approximately

    // Background label
    backgroundGridLabel = new QLabel(ui->centralwidget);
    backgroundGridLabel->setGeometry(0, 0, 800, 600);
    backgroundGridLabel->setAttribute(Qt::WA_TransparentForMouseEvents);


    // Create  pixmap
    backgroundPixmap = new QPixmap();
    backgroundGridLabel->setPixmap(*backgroundPixmap);
    backgroundGridLabel->setScaledContents(true);

    // Bring others forward IF BUTTONS ARE NOT WORKING THIS COULD BE WHY
    ui->textEdit->raise();
    ui->startButton->raise();
    ui->EvaluateButton->raise();
    ui->EvaluateButton->hide();
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
    draggableButtons.push_back(createGateButton(GateType::AND_GATE, ui->actionAndGate->icon()));
    qDebug() << "added AND Gate. There are: " << draggableButtons.size() << "total gates in circuit";
}
void MainWindow::onOrGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::OR_GATE, ui->actionOrGate->icon()));
    qDebug() << "added OR Gate. There are: " << draggableButtons.size() << "total gates in circuit";

}
void MainWindow::onInverterClicked(){
    draggableButtons.push_back(createGateButton(GateType::INVERTER, ui->actionInverter->icon()));
    qDebug() << "added NOT Gate. There are: " << draggableButtons.size() << "total gates in circuit";


}
void MainWindow::onNandGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::NAND_GATE, ui->actionNandGate->icon()));
    qDebug() << "added NAND Gate. There are: " << draggableButtons.size() << "total gates in circuit";

}
void MainWindow::onNorGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::NOR_GATE, ui->actionNorGate->icon()));
    qDebug() << "added NOR Gate. There are: " << draggableButtons.size() << "total gates in circuit";

}
void MainWindow::onXorGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::XOR_GATE, ui->actionXorGate->icon()));
    qDebug() << "added XOR Gate. There are: " << draggableButtons.size() << "total gates in circuit";

}
void MainWindow::onXnorGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::XNOR_GATE, ui->actionXnorGate->icon()));
    qDebug() << "added XNOR Gate. There are: " << draggableButtons.size() << "total gates in circuit";

}
void MainWindow::onInputGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::INPUT, ui->actionInputGate->icon()));
    qDebug() << "added IN Gate. There are: " << draggableButtons.size() << "total gates in circuit";
}


void MainWindow::onOutputGateClicked(){
    draggableButtons.push_back(createGateButton(GateType::OUTPUT, ui->actionOutputGate->icon()));
    qDebug() << "added OUT Gate. There are: " << draggableButtons.size() << "total gates in circuit";

}

void MainWindow::createPhysicsBody(DraggableButton* button) {
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
}

void MainWindow::onClearClicked() {
    // Update all buttons gravity
    for (auto button : draggableButtons) {
        button->buttonDelete();
    }
    // QTimer::singleShot(3000, this, [this]() {draggableButtons.clear();});
    draggableButtons.clear();
    emit clearCircuit();
    qDebug() << "buttons deleted. There are now " << draggableButtons.size() << "buttons left in the cirucuit";
}

DraggableButton* MainWindow::createGateButton(const GateType gateType, const QIcon& icon) {
    Gate* gate = new Gate(gateType);
    DraggableButton* newButton = new DraggableButton(gateType, this, gate);

    emit addButtonToCircuit(newButton, gateType);

    QPoint globalMousePos = QCursor::pos() + QPoint(25,-17);
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

void MainWindow::drawWire(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections) {
    backgroundPixmap->fill(Qt::transparent);

    // Loop through each starting button and its wires
    for (DraggableButton* sourceButton : connections.keys()) {
        QPoint startPos = sourceButton->pos() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);

        // Loop over each connection for source button
        for (const auto& connection : connections.value(sourceButton)) {
            DraggableButton* targetButton = connection.first;
            int inputPort = connection.second;

            // Calculate end position with input port offset
            QPoint endPos = targetButton->pos() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);
            QPoint offset;
            switch (inputPort) {
            case 1: offset = QPoint(-10, -12); break;
            case 2: offset = QPoint(-10, 12); break;
            default: offset = QPoint(0, 0); break;
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
                drawWireArrow(p4, p3, false);
                drawWireArrow(p3, endPos, false);

                // Updated position to draw connecting 3 wires
                endPos = p4;
                startPos = p2;
            }

                int midX = (startPos.x() + endPos.x()) / 2;
                QPoint p1(midX, startPos.y());
                QPoint p2(midX, endPos.y());

                drawWireArrow(startPos, p1, false);
                drawWireArrow(p1, p2, false);
                drawWireArrow(p2, endPos, false);

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


void MainWindow::handleNodeDeleted(DraggableButton* button) {
    // Remove from vector
    draggableButtons.erase(
        std::remove(draggableButtons.begin(), draggableButtons.end(), button),
        draggableButtons.end()
    );
}

void MainWindow::startGame() {
    enableToolBarActions();
    // fix zoom
    backgroundPixmap = new QPixmap(backgroundGridLabel->size());
    backgroundGridLabel->setScaledContents(false);

    ui->startButton->hide();
    // ui->gridLayoutWidget->hide();

    ui->textEdit->hide();
    ui->EvaluateButton->show();
    ui->EvaluateButton->setEnabled(true);
    emit nextLevel(currentLevel);

}
// void MainWindow::startLevel(int level)
// {
//     qDebug() << "Starting level:" << level;
//     currentLevel = level-1;
//     startGame();

// }


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
    ui->actionOutputGate->setEnabled(true);

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
    ui->actionOutputGate->setEnabled(false);

}

// void MainWindow::drawNewLevel(int inputs, TruthTable* newTable) {
    // ui->actionWire->setChecked(false);
    // ui->actionDelete->setChecked(false);
    // enableToolBarActions();

//     QPoint p;

//     backgroundPixmap->fill(Qt::transparent);
//     backgroundGridLabel->setPixmap(*backgroundPixmap);

//     for (int i = 0; i < inputs; i++){
//         DraggableButton* input = createGateButton(GateType::INPUT, ui->actionInputGate->icon());
//         p = QPoint(100, 100*i+ 100);

//         input->move(p);
//         inputOutputButtons.push_back(input);
//     }
//   p = QPoint(600,300);
//     DraggableButton* output = createGateButton(GateType::OUTPUT, ui->actionAndGate->icon());
//     output->move(p);
//     inputOutputButtons.push_back(output);
//     // Pull data out of truthtable
//     // QTableWidget* tableWidget = ui->previewTableWidget;
//     // tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

//     QList tableRows = newTable->getRows();

//     int rowCount = tableRows.size();
//     int inputCount = tableRows[0].first.size();
//     int colCount = inputCount + 1;


//     // tableWidget->setRowCount(rowCount);
//     // tableWidget->setColumnCount(colCount);

//     // Headers
//     QStringList headers;
//     for (int i = 0; i < inputCount; ++i)
//         headers << QString("In%1").arg(i + 1);
//     headers << "Out";
//     // tableWidget->setHorizontalHeaderLabels(headers);

//     // Fill
//     for (int row = 0; row < rowCount; ++row) {
//         const QVector<int>& inputs = tableRows[row].first;
//         int output = tableRows[row].second;

//         for (int col = 0; col < inputCount; ++col) {
//             // QTableWidgetItem* item = new QTableWidgetItem(QString::number(inputs[col]));
//             // item->setTextAlignment(Qt::AlignCenter);
//             // tableWidget->setItem(row, col, item);
//             // ui->previewTableWidget->show();

//         }

//         // QTableWidgetItem* outItem = new QTableWidgetItem(QString::number(output));
//         // outItem->setTextAlignment(Qt::AlignCenter);
//         // tableWidget->setItem(row, inputCount, outItem);
//     }
//     ui->EvaluateButton->show();
//     ui->EvaluateButton->setEnabled(true);
// }

// void MainWindow::getNextLevel(bool levelComplete, TruthTable *currentTable) {
//     if (levelComplete){
//         QMessageBox msgBox;
//         msgBox.setWindowTitle("That's Correct!");
//         msgBox.setIcon(QMessageBox::Warning);
//         msgBox.setText("Wow! Great Job!");
//         msgBox.setInformativeText("Click 'Ok' to continue to the next level!");
//         msgBox.setStandardButtons(QMessageBox::Ok);
//         msgBox.exec();
//         //clean up input + output buttons
//         for(DraggableButton* button: inputOutputButtons){
//             button->hide();
//             button->deleteLater();
//         }
//         inputOutputButtons.clear();

//         currentLevel++;
//         if(currentLevel >= 12){
//             currentLevel = 100;
//         }
//         emit nextLevel(currentLevel);
//     }
//     else
//     {
//         // Display try again message.
//         QMessageBox msgBox;
//         msgBox.setWindowTitle("Incorrect Solution");
//         msgBox.setIcon(QMessageBox::Warning);
//         msgBox.setText("Oops! That solution isn't correct.");
//         msgBox.setInformativeText("Hint: " + currentTable->getHint());
//         msgBox.setStandardButtons(QMessageBox::Ok);
//         msgBox.exec();
//         ui->EvaluateButton->setEnabled(true);

//         return;
//     }
// }

void MainWindow::evaluationAnimation(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections) {
    QVector<QPair<QPoint,QPoint>> wireSegments;

    // Collect all wire segments
    for (DraggableButton* sourceButton : connections.keys()) {
        QPoint startPos = sourceButton->pos() - QPoint(GATE_SIZE/2, -GATE_SIZE/2);

        for (const auto& connection : connections.value(sourceButton)) {
            DraggableButton* targetButton = connection.first;
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

void MainWindow::displayLevelDescription(QString description) {
    // QLabel *textLabel = ui->tipDescription;
    // textLabel->setText(description);
    // textLabel->show();
}

void MainWindow::disableEvaluate() {
    ui->EvaluateButton->setEnabled(false);
}



void MainWindow::on_startButton_clicked()
{
    startGame();
}

void MainWindow::returnToMenu(){
    //clear buttons
    // ui->previewTableWidget->hide();
    ui->EvaluateButton->hide();

    disableToolBarActions();

    // // Create  pixmap
    backgroundPixmap = new QPixmap();

    backgroundGridLabel->setPixmap(*backgroundPixmap);
    backgroundGridLabel->setScaledContents(true);

    // bring others forward IF BUTTONS ARE NOT WORKING THIS COULD BE WHY
    ui->textEdit->raise();
    ui->textEdit->show();
    ui->startButton->show();
    // ui->gridLayoutWidget->show();
    ui->startButton->raise();
    ui->EvaluateButton->raise();
    // ui->gridLayoutWidget->raise();

    //cleanup Buttons
    for( DraggableButton* button : inputOutputButtons){
        button->hide();

    }
    inputOutputButtons.clear();

}

void MainWindow::on_actionMENU_triggered()
{
    returnToMenu();
}


void MainWindow::gameOver() {
    disableToolBarActions();
    ui->EvaluateButton->setEnabled(false);
    QTextEdit *messageBox = new QTextEdit(this);
    messageBox->setReadOnly(true);
    messageBox->setGeometry(150, 150, 450, 300);
    messageBox->setStyleSheet("background-color: white;");
    messageBox->setHtml(
        "<div align='center'>"
        "<h1 style='color:green;'>Congrats, You Won!</h1>"
        "<p style='font-size:14pt;'>You may close out of the game!</p>"
        "</div>"
        );
    messageBox->show();
    ui->actionMENU->setEnabled(false);
    currentLevel =1;
}



