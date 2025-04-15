#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Gate.h"
#include "QVector"
#include "TruthTable.h"
#include "draggablebutton.h"
#include <vector>
#include <QObject>

class Circuit : public QObject {

    Q_OBJECT

public:
    Circuit(QObject *parent = nullptr);
    Circuit();

public slots:
    void addNode(GateType gate);

    // updates mostRecentButton
    void updateButton(DraggableButton *button);

    bool evaluateCircuit();

    //slots
    void onConnectNode(Gate* fromNode, Gate* toNode, int input);
    void onDisconnectNode(Gate* fromNode, Gate* toNode);
    void onDeleteNode(Gate* node);
    void onClear();

    void setTable(QString mode, int level);
    void setTruthTable(const TruthTable& table);


    void registerNode(Gate* node);

    bool hasCycle(Gate* node, QSet<Gate*>& visited, QSet<Gate*>& stack);

    bool isAcyclic(Gate* startNode);

    // Handles connections between Circuit and DraggableButton
    void addButton(DraggableButton *button);


private:

    std::vector<Gate*> inputNodes;

    // Stores all the buttons in the MainWindow
    std::vector<DraggableButton*> allButtons;

    Gate* output;
    QVector<Gate*> gates;

    DraggableButton *mostRecentButton;

    QVector<TruthTable> easyTables;
    QVector<TruthTable> medTables;
    QVector<TruthTable> hardTables;

    void initializeEasyTruthTables();

    void initializeMedTruthTables();

    void initializeHardTruthTables();

    bool evaluateNodeTree(Gate* node);

    TruthTable currTable;



private slots:
    // void onConnectNode(Node* fromNode, Node* toNode, int input);

   // void onDisconnectNode(Node* fromNode, Node* toNode);

   // void onDeleteNode(Node* node);

signals:

    // Emitted to DraggableButton to inform it of the "previous" button
    void mostRecentButtonUpdated(DraggableButton *button);


};

#endif
