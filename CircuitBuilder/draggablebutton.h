#ifndef DRAGGABLEBUTTON_H
#define DRAGGABLEBUTTON_H
#include <QPushButton>
#include <QMouseEvent>
#include <string>
#include <Box2D/Box2D.h>
#include "Gate.h"


class DraggableButton : public QPushButton
{
    Q_OBJECT

private:

    // should be GateType??
    QString buttonType;
    QPoint dragStartPos;
    QPoint currentPos;
    bool onWireMode = false;

    DraggableButton* previousButton = nullptr;

    b2Body* body;

    int GRID_SIZE = 50;
    float PIXELS_PER_METER = 30.0f;

    QPoint snapToGrid(QPoint& point);
    void updatePhysicsBody(QPoint& newPos);

    Gate* gate;


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


public:
    DraggableButton();


    explicit DraggableButton(QString buttonType, QWidget *parent = nullptr);

    DraggableButton(GateType gateType, QWidget *parent = nullptr, Gate* gate = nullptr);


    QPoint getPosition() { return currentPos; }
    void setPosition(QPoint& pos) { currentPos = pos; }
    void setBody(b2Body* newBody) {body = newBody;}
    b2Body* getPhysicsBody();

public slots:
    void setWireMode(bool isWireOn);

    void getTwoButtons(DraggableButton *previousButton);

signals:
    void sendButton(DraggableButton *button);

    void sendTwoButtons(DraggableButton *startButton, DraggableButton *endButton);


};

#endif // DRAGGABLEBUTTON_H
