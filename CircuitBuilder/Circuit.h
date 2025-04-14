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

private:
    QVector<Node> gates;

    DraggableButton *mostRecentButton;

    QVector<TruthTable> easyTables;
    QVector<TruthTable> medTables;
    QVector<TruthTable> hardTables;

    bool evaluateGates();

    void initializeEasyTruthTables();

    void initializeMedTruthTables();

    void initializeHardTruthTables();

signals:
    void mostRecentButtonUpdated(DraggableButton *button);
};

#endif
