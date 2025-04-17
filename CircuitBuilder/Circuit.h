#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Gate.h"
#include "QVector"
#include "TruthTable.h"
#include "draggablebutton.h"
#include <vector>
#include <QObject>
#include <QMap>

class Circuit : public QObject {

    Q_OBJECT

public:
    Circuit(QObject *parent = nullptr);
    Circuit();

public :
    // void addNode(GateType gate);


    bool evaluateCircuit();

    void setTable(QString mode, int level);

    bool hasCycle(Gate* node, QSet<Gate*>& visited, QSet<Gate*>& stack);

    bool isAcyclic(Gate* startNode);

    // updates mostRecentOuput when in wire mode
    void updateOutputButton(DraggableButton *button, int input);

    // Handles connections between Circuit and DraggableButton
    void addButton(DraggableButton *button);

     void onConnectNode(DraggableButton* fromButton, DraggableButton* toButton, int input);


private:

    std::vector<Gate*> inputNodes;

    Gate* output;
    QVector<Gate*> gates;

    //QMap<DraggableButton*, QVector<DraggableButton*>> connections;

    QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections;

    DraggableButton *mostRecentOutput;

    QVector<TruthTable> easyTables;
    QVector<TruthTable> medTables;
    QVector<TruthTable> hardTables;

    void initializeEasyTruthTables();

    void initializeMedTruthTables();

    void initializeHardTruthTables();

    bool evaluateNodeTree(Gate* node);

    TruthTable currTable;



public slots:
    // void onConnectNode(Node* fromNode, Node* toNode, int input);

   // void onDisconnectNode(Node* fromNode, Node* toNode);

   // void onDeleteNode(Node* node);

    void registerGate(DraggableButton* button);

    //void onSendConnections();

    void onButtonMoved(DraggableButton* button);

    //slots
    void onDisconnectNode(DraggableButton* fromButton, DraggableButton* toButton);
    void onDeleteNode(DraggableButton* button);
    void onClear();

signals:

    // Emitted to DraggableButton to inform it of the "previous" button
    // Also sends the identity of the input that requested a connection.
    void mostRecentOutputUpdated(DraggableButton *button, int input);

    void allConnections(QMap<DraggableButton*, QVector<QPair<DraggableButton*, int>>> connections);
};

#endif
