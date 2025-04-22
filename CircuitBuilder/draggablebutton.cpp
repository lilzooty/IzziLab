#include "draggablebutton.h"

DraggableButton::DraggableButton() {
}

DraggableButton::DraggableButton(GateType gateType, QWidget *parent, Gate* gate)
    : QPushButton(parent),
    input1{QPushButton("1", this)}, input2{QPushButton("2", this)}, output{QPushButton("out", this)},
    gate(gate), gateType(gateType), onIcon(":/GATES/INPUT-ON.png"), offIcon(":/GATES/INPUT-OFF.png"), isDelete(false)
    {


    if (gateType == GateType::INVERTER){
        input2.hide();
        input1.move(this->x() +15 , this->y());
        output.move(this->x() + 15, this->y()+12);
        connect(&input1, &QPushButton::clicked, this, &DraggableButton::input1Clicked);
        connect(&output, &QPushButton::clicked, this, &DraggableButton::outputClicked);
    }
    else if(gateType == GateType::INPUT){
        input1.hide();
        input2.hide();
        output.move(this->x() + 15, this->y()+12);
        connect(&output, &QPushButton::clicked, this, &DraggableButton::outputClicked);
    }
    else{

    input1.move(this->x() -10 , this->y());
    input2.move(this->x() -10, this->y() +25);
    output.move(this->x() + 15, this->y()+12);
    connect(&input1, &QPushButton::clicked, this, &DraggableButton::input1Clicked);
    connect(&input2, &QPushButton::clicked, this, &DraggableButton::input2Clicked);
    connect(&output, &QPushButton::clicked, this, &DraggableButton::outputClicked);
    }

}

    DraggableButton::DraggableButton(GateType gateType, QWidget *parent, Gate* gate, QPoint pos)
        : QPushButton(parent),
        input1{QPushButton("1", this)}, input2{QPushButton("2", this)}, output{QPushButton("out", this)},
        gate(gate), gateType(gateType)
    {
        this->setPosition(pos);
        if (gateType == GateType::INVERTER){
            input2.hide();
            input1.move(this->x() +15 , this->y());
            output.move(this->x() + 15, this->y()+12);
            connect(&input1, &QPushButton::clicked, this, &DraggableButton::input1Clicked);
            connect(&output, &QPushButton::clicked, this, &DraggableButton::outputClicked);
        }
        else{

            input1.move(this->x() -10 , this->y());
            input2.move(this->x() -10, this->y() +25);
            output.move(this->x() + 15, this->y()+12);
            connect(&input1, &QPushButton::clicked, this, &DraggableButton::input1Clicked);
            connect(&input2, &QPushButton::clicked, this, &DraggableButton::input2Clicked);
            connect(&output, &QPushButton::clicked, this, &DraggableButton::outputClicked);
        }

    }

void DraggableButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPos = event->pos();
    }


    QPushButton::mousePressEvent(event);

    if (isDelete){
        emit deleteMe(this);
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

void DraggableButton::mouseMoveEvent(QMouseEvent *event)
{
    if (gateType != INPUT && gateType != OUTPUT){
    if (event->buttons() & Qt::LeftButton) {

        QPoint globalPos = mapToParent(event->pos() - dragStartPos);
        QPoint newPos = snapToGrid(globalPos);

        move(newPos);
        updatePhysicsBody(newPos);
    }
    emit onButtonMoved(this);
    QPushButton::mouseMoveEvent(event);
    }
}


void DraggableButton::input1Clicked(){

    if (isDelete){
        qDebug() << isDelete;
        emit sendButton(this, 1, true);
    }
    else if (wireModeOn) {
        emit sendButton(this, 1, false);
    }

}
void DraggableButton::input2Clicked(){

    if (isDelete){
        emit sendButton(this, 2, true);
    }
    else if (wireModeOn) {
        emit sendButton(this, 2, false);
    }

}
void DraggableButton::outputClicked(){
    if (isDelete){
        emit sendButton(this, 3, true);
    }
    else if (wireModeOn) {
        emit sendButton(this, 3, false);
    }

}

QPoint DraggableButton::snapToGrid( QPoint& point)
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

b2Body* DraggableButton::getPhysicsBody()
{
    return body;
}

void DraggableButton::updatePhysicsBody(QPoint& newPos)
{
    if (b2Body* body = getPhysicsBody()) {
        b2Vec2 physicsPos(newPos.x() / PIXELS_PER_METER,
                          newPos.y() / PIXELS_PER_METER);
        body->SetTransform(physicsPos, body->GetAngle());
        body->SetAwake(true);
    }
}

void DraggableButton::setWireMode(bool isWireMode) {
    this->wireModeOn = isWireMode;

}

void DraggableButton::setDeleteMode(bool isDelete){
    this->isDelete = isDelete;
}

void DraggableButton::getTwoButtons(DraggableButton *previousButton, int input) {
    if (wireModeOn) {
        emit sendTwoButtons(previousButton, this, input);
    }
}

Gate* DraggableButton::getGate(){
    return gate;
}


