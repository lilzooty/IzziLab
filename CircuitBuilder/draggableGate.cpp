#include "draggableGate.h"

draggableGate::draggableGate() {
}

draggableGate::draggableGate(GateType gateType, QWidget *parent, Gate* gate) :
    QPushButton(parent),
    gateType(gateType),
    onIcon(":/GATES/INPUT-ON.svg"),
    offIcon(":/GATES/INPUT-OFF.svg"),
    isDelete(false),
    input1{QPushButton("1", this)},
    input2{QPushButton("2", this)},
    output{QPushButton("out", this)},
    gate(gate)
{
    // this->setStyleSheet(mainButtonStyle);

    // input1.setStyleSheet(buttonStyle);
    // input2.setStyleSheet(buttonStyle);
    // output.setStyleSheet(buttonStyle);

    input1.setFixedSize(18, 18);
    input2.setFixedSize(18, 18);
    output.setFixedSize(22, 20);

    input1.move(0, 5);
    input2.move(0, 25);
    output.move(28, 15);


    if (gateType == INVERTER){
        input2.hide();
        input1.move(0, 5);
        connect(&input1, &QPushButton::clicked, this, &draggableGate::input1Clicked);
        connect(&output, &QPushButton::clicked, this, &draggableGate::outputClicked);
    }
    else if(gateType == INPUT){
        input1.hide();
        input2.hide();
        connect(&output, &QPushButton::clicked, this, &draggableGate::outputClicked);
    }
    else if (gateType == OUTPUT){
        input2.hide();
        output.hide();
        connect(&input1, &QPushButton::clicked, this, &draggableGate::input1Clicked);
    }
    else{

        connect(&input1, &QPushButton::clicked, this, &draggableGate::input1Clicked);
        connect(&input2, &QPushButton::clicked, this, &draggableGate::input2Clicked);
        connect(&output, &QPushButton::clicked, this, &draggableGate::outputClicked);
    }

}


void draggableGate::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPos = event->pos();
    }

    QPushButton::mousePressEvent(event);

    if (isDelete){
        buttonDelete();
    }

    if (gateType == INPUT){
        if(this->icon().cacheKey() == onIcon.cacheKey()){
            this->setIcon(offIcon);
        } else{
            this->setIcon(onIcon);
        }
        emit toggleSignal(this);  //TELL IF ON OR OFF
    }
    }

void draggableGate::mouseMoveEvent(QMouseEvent *event)
{
    // if (gateType != INPUT && gateType != OUTPUT){
        if (event->buttons() & Qt::LeftButton) {

            QPoint globalPos = mapToParent(event->pos() - dragStartPos);
            QPoint newPos = snapToGrid(globalPos);

            move(newPos);
            updatePhysicsBody(newPos);
        }
        emit onButtonMoved(this);
        QPushButton::mouseMoveEvent(event);
    }
// }


void draggableGate::input1Clicked(){

    if (isDelete){
        emit sendButton(this, 1, true);
    }
    else if (wireModeOn) {
        emit sendButton(this, 1, false);

    }

}
void draggableGate::input2Clicked(){

    if (isDelete){
        emit sendButton(this, 2, true);

    }
    else if (wireModeOn) {
        emit sendButton(this, 2, false);
    }

}
void draggableGate::outputClicked(){

    if (isDelete){
        emit sendButton(this, 3, true);
    }
    else if (wireModeOn) {
        emit sendButton(this, 3, false);

    }

}

QPoint draggableGate::snapToGrid( QPoint& point)
{
    // assume closest Grid Cell is up and left
    // round to that Grid Cell
    int relativeX = std::round(point.x() / GRID_SIZE) * GRID_SIZE;
    int relativeY = std::round(point.y() / GRID_SIZE) * GRID_SIZE;

    // if click closer to right Cell add
    if (point.x() % GRID_SIZE >  GRID_SIZE/2 ){
        relativeX += GRID_SIZE;
    }

    // if click closer to the bottom add
    if (point.y() % GRID_SIZE >  GRID_SIZE/2 ){
        relativeY += GRID_SIZE;
    }

    return QPoint(relativeX, relativeY);
}

b2Body* draggableGate::getPhysicsBody()
{
    return body;
}

void draggableGate::updatePhysicsBody(QPoint& newPos)
{
    if (b2Body* body = getPhysicsBody()) {
        b2Vec2 physicsPos(newPos.x() / PIXELS_PER_METER,
                          newPos.y() / PIXELS_PER_METER);
        body->SetTransform(physicsPos, body->GetAngle());
        body->SetAwake(true);
    }
}

void draggableGate::setWireMode(bool isWireMode) {
    this->wireModeOn = isWireMode;

}

void draggableGate::setDeleteMode(bool isDelete){
    this->isDelete = isDelete;
}

Gate* draggableGate::getGate(){
    return gate;
}
void draggableGate::setPosition(QPoint &pos){
    if(currentPos != pos)
        emit onButtonMoved(this);
    currentPos = pos;
}
void draggableGate::buttonDelete(){
        // if (gateType != OUTPUT && gateType != INPUT){
            float angle = (float)(rand() % 360) * M_PI / 180.0f;
            float magnitude = 24.0f;
            this->getPhysicsBody()->SetLinearVelocity( b2Vec2(magnitude * cos(angle), magnitude * sin(angle)));

            QTimer::singleShot(1000, this, [this]() {
                emit deleteMe(this);
            });
        // }
}

