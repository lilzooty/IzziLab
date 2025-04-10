#ifndef DRAGGABLEBUTTON_H
#define DRAGGABLEBUTTON_H
#include <QPushButton>
#include <QMouseEvent>
#include <string>
#include <Box2D/Box2D.h>


class DraggableButton : public QPushButton
{
    Q_OBJECT

private:

    QString buttonType;
    QPoint dragStartPos;
    QPoint currentPos;

    b2Body* body;

    int GRID_SIZE = 50;
    float PIXELS_PER_METER = 30.0f;

    QPoint snapToGrid(QPoint& point);
    void updatePhysicsBody(QPoint& newPos);


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


public:
    DraggableButton();
    explicit DraggableButton(QString buttonType, QWidget *parent = nullptr);
    QPoint getPosition() { return currentPos; }
    void setPosition(QPoint& pos) { currentPos = pos; }
    void setBody(b2Body* newBody) {body = newBody;}
    b2Body* getPhysicsBody();

};

#endif // DRAGGABLEBUTTON_H
