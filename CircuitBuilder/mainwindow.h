#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <draggablebutton.h>
#include <QTimer>
#include<Box2D/Box2D.h>
#include <QLabel>
#include "Gate.h"
#include "Circuit.h"

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onAndGateClicked();
    void onOrGateClicked();
    void onInverterClicked();
    void onNandGateClicked();
    void onNorGateClicked();
    void onXorGateClicked();
    void onXnorGateClicked();

    void onWireClicked(bool checked);

    void onClearClicked();
    void updatePhysics();

    void drawWire(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections);


private:
    Ui::MainWindow *ui;
    //physics members
    b2World* physicsWorld;
    QTimer* physicsTimer;
    float pixelsPerMeter = 30.0f;
    int GATE_SIZE = 50;
    vector<vector<bool>> grid;

    // The Model
    Circuit circuit;

    // stores all buttons currently displayed
    vector<DraggableButton*> draggableButtons;

    //physics methods
    void initializePhysics();
    void createPhysicsBody(DraggableButton* button);
    DraggableButton* createGateButton(GateType gateType, const QIcon& icon);

    QLabel* backgroundGridLabel;
    QPixmap* backgroundPixmap;

signals:

    // Notifies the model to add a new Gate from the supplied button
    void addButtonToCircuit(DraggableButton* button, GateType gateType);


};
#endif // MAINWINDOW_H
