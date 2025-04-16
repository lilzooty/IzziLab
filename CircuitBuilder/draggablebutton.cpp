#include "draggablebutton.h"

DraggableButton::DraggableButton() {
}

DraggableButton::DraggableButton(QString buttonType, QWidget *parent)
    : QPushButton(parent), buttonType(buttonType), gate()
{
    this->setToolTip(buttonType);
    setMouseTracking(true);
    this->show();
}

void DraggableButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPos = event->pos();
    }

    QPushButton::mousePressEvent(event);
    emit sendButton(this);
}

void DraggableButton::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

        QPoint globalPos = mapToParent(event->pos() - dragStartPos);
        QPoint newPos = snapToGrid(globalPos);

        move(newPos);
        updatePhysicsBody(newPos);
    }

    QPushButton::mouseMoveEvent(event);
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

        // Wake up the body
        body->SetAwake(true);
    }
}

void DraggableButton::setWireMode(bool isWireMode) {
    onWireMode = isWireMode;
}

void DraggableButton::getTwoButtons(DraggableButton *previousButton) {
    if (onWireMode) {
        emit sendTwoButtons(previousButton, this);
    }
}
