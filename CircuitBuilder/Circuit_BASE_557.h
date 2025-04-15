#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Node.h"
#include "QVector"
#include "TruthTable.h"

class Circuit{
public:
    Circuit();

private:
    QVector<Node> gates;

    QVector<TruthTable> easyTables;
    QVector<TruthTable> medTables;
    QVector<TruthTable> hardTables;

    bool evaluateGates();

    void initializeEasyTruthTables();

    void initializeMedTruthTables();

    void initializeHardTruthTables();
};

#endif
