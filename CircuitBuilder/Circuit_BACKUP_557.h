#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Node.h"
#include "QVector"
#include "TruthTable.h"
#include "draggablebutton.h"
#include <QObject>

class Circuit : public QObject {

    Q_OBJECT

public:
    Circuit(QObject *parent = nullptr);

public slots:
    void addNode(GateType gate);

    void updateButton(DraggableButton *button);

    bool evaluateCircuit(const QVector<Node*>& inputNodes, Node* outputNode, const TruthTable& truthTable);
    bool evaluateCircuit();

    //slots
    void onConnectNode(Node* fromNode, Node* toNode, int input);
    void onDisconnectNode(Node* fromNode, Node* toNode);
    void onDeleteNode(Node* node);
    void onClear();

    void setTable(QString mode, int level);
    void setTruthTable(const TruthTable& table);


    void registerNode(Node* node);

    bool hasCycle(Node* node, QSet<Node*>& visited, QSet<Node*>& stack);

    bool isAcyclic(Node* startNode);

private:

    std::vector<Node*> inputNodes;
    Node* output;
    QVector<Node*> allNodes;

    DraggableButton *mostRecentButton;

    QVector<TruthTable> easyTables;
    QVector<TruthTable> medTables;
    QVector<TruthTable> hardTables;

    void initializeEasyTruthTables();

    void initializeMedTruthTables();

    void initializeHardTruthTables();

<<<<<<< HEAD
    bool evaluateNodeTree(Node* node);

    TruthTable currTable;



private slots:
    // void onConnectNode(Node* fromNode, Node* toNode, int input);

   // void onDisconnectNode(Node* fromNode, Node* toNode);

   // void onDeleteNode(Node* node);
=======
signals:
    void mostRecentButtonUpdated(DraggableButton *button);
>>>>>>> 51c4b0eca310d837c51d2aa310fa7e7b81e5aca3
};

#endif
