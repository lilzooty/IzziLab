#ifndef DRAGGABLEBUTTON_H
#define DRAGGABLEBUTTON_H
#include <QPushButton>
#include <QMouseEvent>
#include <QIcon>
#include <QTimer>
#include <Box2D/Box2D.h>
#include "Gate.h"


class DraggableButton : public QPushButton
{
    Q_OBJECT

private:

    GateType gateType;
    QIcon const onIcon;
    QIcon const offIcon;

    QPoint dragStartPos;
    QPoint currentPos;
    bool wireModeOn = false;
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
    QString buttonStyle =
        "QPushButton {"
        "    background-color: transparent;"
        "    border: 1px solid transparent;"
        "    border-radius: 7px;"
        "    color: white;"
        "    min-width: 15px;"
        "    min-height: 15px;"
        "    padding: 0px;"
        "}"
        "QPushButton:hover {"
        "    border: 1px solid white;"
        "    background-color: rgba(255, 255, 255, 30);"
        "}";

    QString mainButtonStyle =
        "QPushButton {"
        "    background-color: rgba(40, 40, 60, 150);"
        "    border: 2px solid rgba(80, 100, 255, 120);"
        "    border-radius: 8px;"
        "    padding: 5px;"

        "}"
        "QPushButton:hover {"
        "    background-color: rgba(50, 50, 70, 170);"
        "    border: 2px solid purple;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(35, 35, 55, 160);"
        "    border: 2px solid rgba(60, 80, 255, 120);"
        "}";


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


public:
    DraggableButton();

    void buttonDelete();


    DraggableButton(GateType gateType, QWidget *parent = nullptr, Gate* gate = nullptr);

    QPoint getPosition() { return currentPos; }
    void setPosition(QPoint& pos);
    void setBody(b2Body* newBody) {body = newBody;}
    b2Body* getPhysicsBody();

    Gate* getGate();




public slots:
    void setWireMode(bool isWireOn);
    void setDeleteMode(bool isDelete);



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

    void onButtonMoved(DraggableButton* button);

    void deleteMe(DraggableButton*);

    void toggleSignal(DraggableButton*);


};

#endif // DRAGGABLEBUTTON_H
