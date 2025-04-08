#include "draggablebutton.h"
#include <QMouseEvent>

DraggableButton::DraggableButton() {

}

DraggableButton::DraggableButton(QString buttonType, QWidget *parent)
    : QPushButton(parent), buttonType(buttonType)
{
    this->setText(buttonType);
    setMouseTracking(true);
    this->show();
}

void DraggableButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPos = event->pos();
    }
    QPushButton::mousePressEvent(event);
}

void DraggableButton::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - dragStartPos;
        move(pos() + diff);
    }
    QPushButton::mouseMoveEvent(event);
}
