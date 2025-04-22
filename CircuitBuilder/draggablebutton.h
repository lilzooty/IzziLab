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

    GateType gateType;


    QPoint dragStartPos;
    QPoint currentPos;
    bool wireModeOn = false;

    // // never used?
    // DraggableButton* previousButton = nullptr;


    QPushButton input1;
    QPushButton input2;
    QPushButton output;


    bool isDelete;

    // Needed for physics
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


    //explicit DraggableButton(QString buttonType, QWidget *parent = nullptr);

    DraggableButton(GateType gateType, QWidget *parent = nullptr, Gate* gate = nullptr);


    QPoint getPosition() { return currentPos; }
    void setPosition(QPoint& pos) { currentPos = pos; }
    void setBody(b2Body* newBody) {body = newBody;}
    b2Body* getPhysicsBody();

    Gate* getGate();



public slots:
    void setWireMode(bool isWireOn);
    void setDeleteMode(bool isDelete);

    void getTwoButtons(DraggableButton *previousButton, int input);



    void input1Clicked();
    void input2Clicked();
    void outputClicked();

signals:

    /**
     * @brief sendButton
     * @param button
     * @param input 1 if input1, 2 if input2, 3 if output.
     */
    void sendButton(DraggableButton *button, int input, bool deletingWire);

    void sendTwoButtons(DraggableButton *startButton, DraggableButton *endButton, int input);
    void onButtonMoved(DraggableButton* button);

    void deleteMe(DraggableButton*);

    void toggleSignal(DraggableButton*);


};

#endif // DRAGGABLEBUTTON_H
