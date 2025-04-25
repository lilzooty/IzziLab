#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Gate.h"
#include "QVector"
#include "TruthTable.h"
#include "draggablebutton.h"
#include <vector>
#include <QTimer>
#include <QObject>
#include <QMap>

/**
 * @brief The Circuit class represents the circuit that the user will build. Acts as the model for our program. Handles the evaluation
 * of the Circuits the the user will make.
 */
class Circuit : public QObject {

    Q_OBJECT

public:
    /**
     * @brief Constructor that builds up based on a parent QObject.
     * @param parent - The parent QObject that this widget will be linked to.
     */
    Circuit(QObject *parent = nullptr);

    /**
     * @brief Base constructor that is initalized without a parent object.
     */
    Circuit();

public :
    /**
     * @brief This method evaluates the circuit at the state that the user has constructed it to see if it matches the expected output.
     * @return - True if it is correct, false if not.
     */
    bool evaluateCircuit();

    /**
     * @brief This sets the table of expected output for a circuit based on difficulty and the level.
     * @param mode - Difficulty Mode.
     * @param level - Level that the user is on.
     */
    void setTable(QString mode, int level);

    /**
     * @brief Checks if the circuit has a cycle.
     * @param node - Node to check if it is within a cycle.
     * @param visited - Set of visited nodes so that it doesn't visit the same one more than once.
     * @param stack - Set of nodes that can be in a cycle.
     * @return - True if a cycle is detected, false if not.
     */
    bool hasCycle(Gate* node, QSet<Gate*>& visited, QSet<Gate*>& stack);

    /**
     * @brief Checks to see if the circuit is not cyclical.
     * @param startNode - The node to start on to check for no cycles.
     * @return - True if there isn't a cycle, false if not.
     */
    bool isAcyclic(Gate* startNode);

    /**
     * @brief Updates the most recent output button clicked for a node to keep track of connections.
     * @param button - Button that was pressed.
     * @param input - Which input was pressed.
     * @param deletingWire - if the connection should be deleted
     */
    void updateOutputButton(DraggableButton *button, int input, bool deletingWire);

    /**
     * @brief Adds a button to the circuits vector of buttons through a connection with draggable button.
     * @param button - Button to be added.
     */
    void addButton(DraggableButton *button);

    /**
      * @brief Handles the connections of different nodes within the builder to keep track of.
      * @param fromButton - Root connection button.
      * @param toButton - Child connection button.
      * @param input - Which input on the child button.
      */
     void onConnectNode(DraggableButton* fromButton, DraggableButton* toButton, int input);

private:
     /**
      * @brief Starts at 0, goes up to 20. Represents current game level.
      */
     int currentLevel = 0;

     /**
     * @brief The child nodes.
     */
    std::vector<Gate*> inputNodes;

    /**
     * @brief The output node of the circuit. Used to traverse the circuit graph structure
     * for evaluation.
     */
    Gate* output;

    /**
     * @brief List of gates to be used for evaluation.
     */
    QVector<Gate*> gates;

    /**
     * @brief This represents the connections of nodes which will be used to evaluate.
     */
    QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections;

    /**
     * @brief The most recently clicked output/root button clicked so that we can connect the two if a inpput/child button is clicked.
     */
    DraggableButton *mostRecentOutput = nullptr;

    /**
     * @brief Table of easy circuit problems.
     */
    QVector<TruthTable> allTables;

    /**
     * @brief Initializes an easy truth table for a level.
     */
    void initializeTruthTables();

    /**
     * @brief Evaluates the current state of the ciruit program that the user has.
     * @param node - Starting node to start evaluation.
     * @return - True if correct, false if not.
     */
    bool evaluateNodeTree(Gate* node);

    /**
     * @brief Current table that is being used to challenge player.
     */
    TruthTable currTable;

public slots:

    /**
     * @brief Registers a gate to the vector of gates.
     * @param button
     */
    void registerGate(DraggableButton* button);

    /**
     * @brief slot for when a button is moved. This method sends a signal of the
     * button connections.
     */
    void onButtonMoved();

    /**
     * @brief When a node is disconnected, it needs to be removed from vector of connections.
     * @param fromButton - Root/output button.
     * @param toButton - Child/input button.
     * @param input - the connection on the gate to be disconnected
     */
    void onDisconnectNode(DraggableButton* fromButton, DraggableButton* toButton, int input);

    /**
     * @brief When a node is deleted we need to update the state of the model.
     * @param button - Button to be deleted.
     */
    void onDeleteNode(DraggableButton* button);

    /**
     * @brief When a clear is pressed, we need to reset the program to it's inital state.
     */
    void onClear();

    /**
     * @brief Changes the output of an input gate when its button is clicked.
     */
    void toggleInputSignal(DraggableButton* inputButton);

    /**
     * @brief handles the actual logic of creating a new level and sends it to mainwindow
     */
    void levelUp(int level);

    /**
     * Helper to make levels, emits sendInputCount when level is created.
     */
    int getInputButtonCount(int currentLevel);

    /**
     * @brief Used to send the animation and connections list to mainwindow for wire drawing.
     */
    void onEvaluate();

signals:

    /**
     * @brief Informs the state of the model that the most recent output button has been updated.
     * @param button - Button that is the new output/root button.
     * @param input - Input of the child node.
     */
    void mostRecentOutputUpdated(DraggableButton *button, int input);

    /**
     * @brief send to Mainwindow to indicate that the node is deleted so that mainwindow can remove from its vector of buttons.
     */
    void nodeDeleted(DraggableButton* button);

    /**
     * @brief Animates the circuit being solved.
     */
    void evaluationAnimation(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections);

    /**
     * @brief Sent to MainWindow. Used to indicate every connection a DraggableButton has so wires
     * can be drawn.
     */
    void allConnections(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections);

    /**
     * Sends the info need to create a new level in mainwindow
     * @param inputs The number of input gates required for the level.
     * @param newTable The TruthTable used to indicate the correct answer.
     */
    void sendLevel(int inputs, TruthTable* newTable);

    /**
     * @brief Sent upon completing circuit evaluation.
     * @param correct True if user circuit is equivalent to the correct answer.
     * @param currentTable
     */
    void sendEvaluation(bool correct, TruthTable *currentTable);

    /**
     * @brief Sends a description for a particular level.
     */
    void sendDescription(QString);
};

#endif
