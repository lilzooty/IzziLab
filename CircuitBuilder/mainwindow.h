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

    /**
     * @brief Slot that tells the Circuit that the user is in wire/connection mode.
     * @param checked - If the wire toolbar action is checked or not.
     */
    void onWireClicked(bool checked);

    /**
     * @brief Slot that tells the Circuit that the user is in delete mode.
     * @param checked - If the delete toolbar action is checked or not.
     */
    void onDeleteClicked(bool checked);

    /**
     * @brief If the clear action in the toolbar is clicked to clear the circuit area.
     */
    void onClearClicked();

    /**
     * @brief Tells the main window to update the physics on a clear.
     */
    void updatePhysics();

    /**
     * @brief Draws a wire to visualize a connection between two nodes.
     * @param connections - Vector of connections that the connection will be added to.
     */
    void drawWire(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections);

    /**
     * @brief When a node is deleted, this removes it from the window.
     * @param button - Button/node to be deleted.
     */
    void handleNodeDeleted(DraggableButton* button);


private:
    /**
     * @brief The User Interface for our mainwindow.
     */
    Ui::MainWindow *ui;

    /**
     * @brief All of our physics members.
     */
    b2World* physicsWorld;
    QTimer* physicsTimer;
    float pixelsPerMeter = 30.0f;
    int GATE_SIZE = 50;
    vector<vector<bool>> grid;

    // The Model
    /**
     * @brief Our circuit model which will be used to update mainwindow.
     */
    Circuit circuit;

    // stores all buttons currently displayed
    /**
     * @brief Stores all of the buttons currently displayed.
     */
    vector<DraggableButton*> draggableButtons;

    //physics methods
    /**
     * @brief Initalizes the physics for the buttons on a clear.
     */
    void initializePhysics();

    /**
     * @brief Creates a physics body with a button.
     * @param button - Button to become a physics body.
     */
    void createPhysicsBody(DraggableButton* button);

    /**
     * @brief Creates a button based on a gatetype to be added to the member variable.
     * @param gateType - Type of the Gate that will be created.
     * @param icon - Image to put over top of the button.
     * @return - A draggable button based on the gatetype.
     */
    DraggableButton* createGateButton(GateType gateType, const QIcon& icon);

    /**
     * @brief QLabel that represents the background where our grid is and will have a pixmap overtop of it.
     */
    QLabel* backgroundGridLabel;

    /**
     * @brief Pixmap that is placed on the QLabel that is used to draw wires.
     */
    QPixmap* backgroundPixmap;

signals:

    // Notifies the model to add a new Gate from the supplied button
    /**
     * @brief Notifies the model to add a new Gate object based on the provided button.
     * @param button - Button two create a gate from.
     * @param gateType - Gate type to be used to create a Gate.
     */
    void addButtonToCircuit(DraggableButton* button, GateType gateType);


};
#endif // MAINWINDOW_H
