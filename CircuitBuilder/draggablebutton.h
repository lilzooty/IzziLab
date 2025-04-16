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

    // never used?
    DraggableButton* previousButton = nullptr;


    QPushButton input1;
    QPushButton input2;

    QPushButton output;




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

    void getTwoButtons(DraggableButton *previousButton, int input);

    void input1Clicked(bool checked);
    void input2Clicked(bool checked);
    void outputClicked(bool checked);

signals:

    /**
     * @brief sendButton
     * @param button
     * @param input 1 if input1, 2 if input2, 3 if output.
     */
    void sendButton(DraggableButton *button, int input);

    void sendTwoButtons(DraggableButton *startButton, DraggableButton *endButton, int input);


};

#endif // DRAGGABLEBUTTON_H
