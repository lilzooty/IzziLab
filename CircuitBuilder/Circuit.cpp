#include "Circuit.h"
#include "qdebug.h"

Circuit::Circuit() : inputNodes(), output(nullptr), gates{}{
    initializeEasyTruthTables();
    initializeMedTruthTables();
    initializeHardTruthTables();
}

Circuit::Circuit(QObject *parent) : QObject{parent}, gates{} {
    initializeEasyTruthTables();
    initializeMedTruthTables();
    initializeHardTruthTables();
}

void Circuit::initializeEasyTruthTables(){
    // Lvl 1: input → output
    QVector<QVector<int>> input1 = { {0}, {1} };

    TruthTable t1(input1, { 0, 1 });
    t1.setHint("Direct pass through: output = input");
    easyTables.append(t1);

    // Lvl 2: inverter (NOT gate)
    TruthTable t2(input1, { 1, 0 });
    t2.setHint("output = not input");
    easyTables.append(t2);
}

void Circuit::initializeMedTruthTables() {
    QVector<QVector<int>> inputs2 = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };

    // Lvl 1: AND gate
    TruthTable t1(inputs2, {0, 0, 0, 1});
    t1.setHint("True when both inputs are 1");
    medTables.append(t1);

    // Lvl 2: OR gate
    TruthTable t2(inputs2, {0, 1, 1, 1});
    t2.setHint("True when either input1 is 1 or input2 is 1");
    medTables.append(t2);

    // Lvl 3: NAND gate
    TruthTable t3(inputs2, {1, 1, 1, 0});
    t3.setHint("True when input1 and inputs2 are both not 1");
    medTables.append(t3);

    // Lvl 4: NOR gate
    TruthTable t4(inputs2, {1, 0, 0, 0});
    t4.setHint("Opposite of an OR gate");
    medTables.append(t4);

    // Lvl 5: XOR gate
    TruthTable t5(inputs2, {0, 1, 1, 0});
    t5.setHint("Inputs must be different");
    medTables.append(t5);

    // Lvl 6: XNOR gate
    TruthTable t6(inputs2, {1, 0, 0, 1});
    t6.setHint("Inputs must be the same");
    medTables.append(t6);

    // Medium hard problems

    // Lvl 7: A AND NOT B
    TruthTable t7(inputs2, {0, 0, 1, 0});
    t7.setHint("Use an AND gate and a NOT gate");
    medTables.append(t7);

    // Lvl 8: NOT A OR B
    TruthTable t8(inputs2,  {1, 1, 0, 1});
    t8.setHint("Use a NOT gate and an OR gate");
    medTables.append(t8);

    // Lvl 9: A XOR B AND A
    TruthTable t9(inputs2, {0, 0, 1, 0});
    t9.setHint("Use an AND gate and a XOR gate");
    medTables.append(t9);
}

void Circuit::initializeHardTruthTables(){
    QVector<QVector<int>> inputs3 = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        {0, 1, 1},
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 0},
        {1, 1, 1}
    };

    // Lvl 1: A AND B OR C
    TruthTable t1(inputs3,{0, 1, 0, 1, 1, 1, 1, 1} );
    t1.setHint("Use AND gate and OR gate");
    hardTables.append(t1);

    // Lvl 2: NOT (A OR B) AND C 
    TruthTable t2(inputs3,{0, 1, 0, 0, 0, 0, 0, 0} );
    t2.setHint("Use NOT gate, OR GATE, and AND gate");
    hardTables.append(t2);

    // Lvl 3: A XOR B XOR C
    TruthTable t3(inputs3,{0, 1, 1, 0, 1, 0, 0, 1} );
    t3.setHint("Use 2 XOR gates");
    hardTables.append(t3);

    // Lvl 4: A AND B XOR B AND C
    TruthTable t4(inputs3,{0, 0, 0, 1, 0, 1, 1, 0} );
    t4.setHint("Use 2 AND gates and a XOR gate");
    hardTables.append(t4);

    // Lvl 5: NOT (A AND B OR NOT C)
    TruthTable t5(inputs3,{1, 1, 1, 1, 1, 1, 0, 0} );
    t5.setHint("2 NOT gates, and AND gate, and an OR gate");
    hardTables.append(t5);

    // Lvl 6: A OR NOT B AND B OR NOT C
    TruthTable t6(inputs3,{1, 1, 1, 1, 1, 1, 1, 1});
    t6.setHint("Use 2 OR Gates, 2 NOT gates, and an AND gate");
    hardTables.append(t6);

    // Lvl 7: Majority function (at least 2 inputs are 1)
    TruthTable t7(inputs3,{0, 0, 0, 1, 0, 1, 1, 1});
    t7.setHint("2 inputs have to be 1");
    hardTables.append(t7);

    // Lvl 8: A NAND B NOR B NAND C  
    TruthTable t8(inputs3,{0, 0, 0, 0, 0, 0, 0, 1});
    t8.setHint("Combine NAND and NOR gates");
    hardTables.append(t8);
}

bool Circuit::evaluateCircuit() {
    if(!isAcyclic(output) || inputNodes.empty() || !output){
        return false;
    }

    for (Gate* gate : gates) {
        if (gate) gate->reset();
    }

    // Get the truth table rows
    QList<QPair<QVector<int>, int>> rows = currTable.getRows();
    if(rows.isEmpty()){
        return false;
    }

    // For each row in the truth table
    for (const auto& row : rows) {
        const QVector<int>& inputs = row.first;
        const bool expectedOutput = row.second;

        // Set input signals
        for (int i = 0; i < inputs.size(); ++i) {
            inputNodes[i]->setSignal(inputs[i] == 1);
        }

        if (!evaluateNodeTree(output)) {
            return false;
        }

        // Get the final output and compare with expected
        bool actualOutput = output->getSignal();

        if(actualOutput != expectedOutput){
            return false;
        }
    }
    return true;
}

bool Circuit::evaluateNodeTree(Gate* node) {
    if(!node){
        return false;
    }

    GateType gateType = node->getGateType();

    if (gateType == INPUT) {
        return true;
    }

    //Check if 1 input gate has 1 input
    if(gateType == INVERTER || gateType == OUTPUT){
        if(!node->getInput1()){
            return false;
        }
    }
    else{
        if(!node->getInput1() || !node->getInput2()){
            return false;
        }
    }

    // Recursively evaluate input nodes first
    if (node->getInput1() != nullptr && !evaluateNodeTree(node->getInput1())) {
        return false;
    }

    if (node->getInput2() != nullptr && !evaluateNodeTree(node->getInput2())) {
        return false;
    }

    // Evaluate this node
    return node->evaluate();
}

void Circuit::onConnectNode(DraggableButton* fromButton, DraggableButton* toButton, int input) {
    if(!fromButton || !toButton){
        return;
    }

    Gate* fromNode = fromButton->getGate();
    Gate* toNode = toButton->getGate();

    if (!fromNode || !toNode || fromNode->getGateType() == OUTPUT || toNode->getGateType() == INPUT) return;

    if (toNode->addInput(fromNode, input)) {


    //GEEKER
    // we dont want to add twice to vector

    }


   connections[fromButton].append(QPair<DraggableButton*, int>(toButton,input));

    qDebug() << connections[fromButton].size();

}

void Circuit::onDisconnectNode(DraggableButton* fromButton, DraggableButton* toButton) {
    Gate* fromNode = fromButton->getGate();
    Gate* toNode = toButton->getGate();

    if (!fromNode || !toNode) return;

    toNode->removeInput(fromNode);
    fromNode->removeOutput(toNode);

    if (connections.contains(fromButton)) {
        QVector<QPair<DraggableButton*, int>>& vec = connections[fromButton];
        QVector<QPair<DraggableButton*, int>> toRemove;

        for (const auto& pair : vec) {
            if (pair.first == toButton) {
                toRemove.append(pair);
            }
        }
        for (const auto& pair : toRemove) {
            vec.removeAll(pair);
        }
    }
}


void Circuit::onDeleteNode(DraggableButton* button){
    if(!button){
        return;
    }
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        QVector<QPair<DraggableButton*, int>>& targets = it.value();
        QVector<QPair<DraggableButton*, int>> toRemove;

        for (const auto& pair : targets) {
            if (pair.first == button) {
                toRemove.append(pair);
            }
        }

        for (const auto& pair : toRemove) {
            targets.removeAll(pair);
        }

        if (targets.isEmpty()) {
            connections.remove(it.key());
        }
    }

    Gate* gate = button->getGate();
    if (gate) {
        gates.removeAll(gate);
        delete gate;
    }
}

void Circuit::onClear() {
    for (Gate* node : gates) {
        if(node){
            node->disconnectAll();
            delete node;
        }
    }
    gates.clear();
    inputNodes.clear();
    connections.clear();
    output = nullptr;
}

void Circuit::registerGate(DraggableButton* button) {
    Gate* node = button->getGate();
    if (!node || gates.contains(node)) {
        return;
    }

    gates.append(node);

    if (node->getGateType() == INPUT) {
        inputNodes.push_back(node);
    } else if (node->getGateType() == OUTPUT) {
        output = node;
    }
    connections[button];
}

bool Circuit::hasCycle(Gate* node, QSet<Gate*>& visited, QSet<Gate*>& stack) {
    if (!node) return false;

    if (stack.contains(node)) return true;
    if (visited.contains(node)) return false;

    visited.insert(node);
    stack.insert(node);

    if (hasCycle(node->getInput1(), visited, stack) ||
        hasCycle(node->getInput2(), visited, stack)) {
        return true;
    }

    stack.remove(node);
    return false;
}

bool Circuit::isAcyclic(Gate* startNode) {
    QSet<Gate*> visited, recStack;
    return !hasCycle(startNode, visited, recStack);
}

void Circuit::setTable(QString mode, int level){
    if(mode == "easy"){
        currTable = easyTables.at(level);
    }
    if(mode == "medium"){
        currTable = medTables.at(level);
    }

    if(mode == "hard"){
        currTable = hardTables.at(level);
    }
}

// void Circuit::addNode(GateType gate) {
//     Gate n(gate);
//     gates.push_back(&n);
// }



void Circuit::updateOutputButton(DraggableButton *button, int input) {
    if (input == 3) {
        mostRecentOutput = button;
    } else if (input == 1 || input == 2) {
        if (mostRecentOutput != nullptr){

            // make a connection between most recent ouput and this button
            onConnectNode(mostRecentOutput, button ,input);

            emit mostRecentOutputUpdated(mostRecentOutput, input);

            // maybe even emit connections list now?
            emit allConnections(connections);
        }

    }
}

void Circuit::addButton(DraggableButton *button){

    registerGate(button);

    connect(button, &DraggableButton::sendButton, this, &Circuit::updateOutputButton);
    connect(this, &Circuit::mostRecentOutputUpdated, button, &DraggableButton::getTwoButtons);

    connect(button, &DraggableButton::onButtonMoved, this, &Circuit::onButtonMoved);  // TO REDRAW WIRES

    qDebug() << "new button";
}

// void Circuit::onSendConnections(){
// qDebug() << "wire";
//     qDebug() << connections;
//   emit allConnections(connections);
// }

void Circuit::onButtonMoved(DraggableButton* button){
    emit allConnections(connections);
}
