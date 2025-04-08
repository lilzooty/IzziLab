#ifndef DRAGGABLEBUTTON_H
#define DRAGGABLEBUTTON_H
#include <QPushButton>
#include <string>

class DraggableButton : QPushButton
{
    Q_OBJECT

private:

    QString buttonType;
    QPoint dragStartPos;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


public:
    DraggableButton();

    explicit DraggableButton(QString buttonType, QWidget *parent = nullptr);

};

#endif // DRAGGABLEBUTTON_H
