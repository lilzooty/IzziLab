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

/**
 * @brief The MainWindow class represents the view of our program and is the part of the program that the user will interact with
 * to build circuits.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window.
     * @param parent - If the mainwindow has a parent QObject that it resides in.
     */
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    /**
     * @brief Slot that creates a draggable button/node that represents an AND logic gate.
     */
    void onAndGateClicked();

    /**
     * @brief Slot that creates a draggable button/node that represents an OR logic gate.
     */
    void onOrGateClicked();

    /**
     * @brief Slot that creates a draggable button/node that represents an inverter gate.
     */
    void onInverterClicked();

    /**
     * @brief Slot that creates a draggable button/node that represents an NAND logic gate.
     */
    void onNandGateClicked();

    /**
     * @brief Slot that creates a draggable button/node that represents an NOR logic gate.
     */
    void onNorGateClicked();

    /**
     * @brief Slot that creates a draggable button/node that represents an XOR logic gate.
     */
    void onXorGateClicked();

    /**
     * @brief Slot that creates a draggable button/node that represents an XNOR logic gate.
     */
    void onXnorGateClicked();


    void onWireClicked(bool checked);

    void onDeleteClicked(bool checked);

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
