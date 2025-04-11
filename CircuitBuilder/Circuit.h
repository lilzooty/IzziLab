#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Node.h"
#include "QVector"
#include "QHash"

class Circuit{
public:
    Circuit();

private:
    QVector<Node> gates;
    QHash<QVector<int>, int> easyTruthTable;
    QHash<QVector<int>, int> medTruthTable;
    QHash<QVector<int>, int> hardTruthTable;

    bool evaluateGates();

};

