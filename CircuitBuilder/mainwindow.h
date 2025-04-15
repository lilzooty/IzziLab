#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <draggablebutton.h>
#include <QTimer>
#include<Box2D/Box2D.h>
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

    void onWireClicked();
    void onClearClicked();
    void updatePhysics();

    void drawWire(DraggableButton *startButton, DraggableButton *endButton);

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

    vector<DraggableButton*> andGates;
    vector<DraggableButton*> orGates;
    vector<DraggableButton*> inverters;
    vector<DraggableButton*> norGates;
    vector<DraggableButton*> nandGates;
    vector<DraggableButton*> xorGates;
    vector<DraggableButton*> xnorGates;

    vector<DraggableButton*> draggableButtons;

    //physics methods
    void initializePhysics();
    void createPhysicsBody(DraggableButton* button);
    DraggableButton* createGateButton(const QString& gateType, const QIcon& icon);

signals:

    void addNode(GateType gate);

    void addButtonToCircuit(DraggableButton *button);

};
#endif // MAINWINDOW_H
