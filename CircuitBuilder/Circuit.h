#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Node.h"
#include "QVector"
#include "TruthTable.h"

class Circuit{
public:
    Circuit();

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

    QVector<TruthTable> easyTables;
    QVector<TruthTable> medTables;
    QVector<TruthTable> hardTables;

    void initializeEasyTruthTables();

    void initializeMedTruthTables();

    void initializeHardTruthTables();

    bool evaluateNodeTree(Node* node);

    TruthTable currTable;



private slots:
    // void onConnectNode(Node* fromNode, Node* toNode, int input);

   // void onDisconnectNode(Node* fromNode, Node* toNode);

   // void onDeleteNode(Node* node);
};

#endif
