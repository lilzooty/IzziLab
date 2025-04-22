#include "Circuit.h"
#include "qdebug.h"

Circuit::Circuit() : inputNodes(), output(nullptr), gates{}{
    initializeTruthTables();
}

Circuit::Circuit(QObject *parent) : QObject{parent}, gates{} {
    initializeTruthTables();
}

void Circuit::initializeTruthTables(){
    QVector<QVector<int>> input1 = { {0}, {1} };

    TruthTable t1(input1, { 0, 1 }, "Direct pass through: output = input");
    allTables.append(t1);

    TruthTable t2(input1, { 1, 0 }, "output = not input");
    allTables.append(t2);

    QVector<QVector<int>> inputs2 = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };

    allTables.append(TruthTable(inputs2, {0, 0, 0, 1}, "True when both inputs are 1"));
    allTables.append(TruthTable(inputs2, {0, 1, 1, 1}, "True when either input1 is 1 or input2 is 1"));
    allTables.append(TruthTable(inputs2, {1, 1, 1, 0}, "True when input1 and inputs2 are both not 1"));
    allTables.append(TruthTable(inputs2, {1, 0, 0, 0}, "Opposite of an OR gate"));
    allTables.append(TruthTable(inputs2, {0, 1, 1, 0}, "Inputs must be different"));
    allTables.append(TruthTable(inputs2, {1, 0, 0, 1}, "Inputs must be the same"));
    allTables.append(TruthTable(inputs2, {0, 0, 1, 0}, "Use an AND gate and a NOT gate"));
    allTables.append(TruthTable(inputs2, {1, 1, 0, 1}, "Use a NOT gate and an OR gate"));
    allTables.append(TruthTable(inputs2, {0, 0, 1, 0}, "Use an AND gate and a XOR gate"));

    QVector<QVector<int>> inputs3 = {
        {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
        {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
    };

    allTables.append(TruthTable(inputs3, {0, 1, 0, 1, 1, 1, 1, 1}, "Use AND gate and OR gate"));
    allTables.append(TruthTable(inputs3, {0, 1, 0, 0, 0, 0, 0, 0}, "Use NOT gate, OR GATE, and AND gate"));
    allTables.append(TruthTable(inputs3, {0, 1, 1, 0, 1, 0, 0, 1}, "Use 2 XOR gates"));
    allTables.append(TruthTable(inputs3, {0, 0, 0, 1, 0, 1, 1, 0}, "Use 2 AND gates and a XOR gate"));
    allTables.append(TruthTable(inputs3, {1, 1, 1, 1, 1, 1, 0, 0}, "2 NOT gates, and AND gate, and an OR gate"));
    allTables.append(TruthTable(inputs3, {1, 1, 1, 1, 1, 1, 1, 1}, "Use 2 OR Gates, 2 NOT gates, and an AND gate"));
    allTables.append(TruthTable(inputs3, {0, 0, 0, 1, 0, 1, 1, 1}, "2 inputs have to be 1"));
    allTables.append(TruthTable(inputs3, {0, 0, 0, 0, 0, 0, 0, 1}, "Combine NAND and NOR gates"));
}

bool Circuit::evaluateCurrentInputs(){
    if(!isAcyclic(output) || inputNodes.empty() || !output){
        return false;
    }
    if(evaluateNodeTree(output)){
        return output->getSignal();
    }

    return false;
}


bool Circuit::evaluateCircuit() {
    qDebug() << "inEval";
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
    if(!fromButton || !toButton || fromButton == toButton) { // Do not allow circular connections
        return;
    }

    Gate* fromNode = fromButton->getGate();
    Gate* toNode = toButton->getGate();

    if (!fromNode || !toNode || fromNode->getGateType() == OUTPUT || toNode->getGateType() == INPUT) return;

    if (toNode->addInput(fromNode, input)) {
        // prevent stacking connections
        QPair<DraggableButton*, int> newPair(toButton,input);
        if(!connections[fromButton].contains(newPair)){
            connections[fromButton].append(newPair);
        }


    }

    qDebug() << connections[fromButton].size();

}

void Circuit::onDisconnectNode(DraggableButton* fromButton, DraggableButton* toButton, int input) {
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
            if( pair.second == input){
                 vec.removeAll(pair);
            }
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
    }

    connections.remove(button);

    Gate* gate = button->getGate();
    if (gate) {
        gates.removeAll(gate);
        delete gate;
    }
    //button->hide();
    button->deleteLater();
    emit nodeDeleted(button);

    emit allConnections(connections); // Redraaw Wires
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

// void Circuit::addNode(GateType gate) {
//     Gate n(gate);
//     gates.push_back(&n);
// }



void Circuit::updateOutputButton(DraggableButton *button, int input, bool deletingWire) {
    if (input == 3) {
        mostRecentOutput = button;
    } else if (input == 1 || input == 2) {
        if (mostRecentOutput != nullptr){

            if( deletingWire){
                onDisconnectNode(mostRecentOutput, button, input);
            }

            // make a connection between most recent ouput and this button
            else {
                onConnectNode(mostRecentOutput, button ,input);
            }
            emit mostRecentOutputUpdated(mostRecentOutput, input);

            //  emit connections
            emit allConnections(connections);
        }
    }
}

void Circuit::addButton(DraggableButton *button){

    registerGate(button);

    connect(button, &DraggableButton::sendButton, this, &Circuit::updateOutputButton);
    connect(this, &Circuit::mostRecentOutputUpdated, button, &DraggableButton::getTwoButtons);

    connect(button, &DraggableButton::onButtonMoved, this, &Circuit::onButtonMoved);  // TO REDRAW WIRES

    connect(button, &DraggableButton::deleteMe, this, &Circuit::onDeleteNode);
    connect(button, &DraggableButton::toggleSignal  , this, &Circuit::toggleInputSignal);


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


int Circuit::getInputButtonCount(int level){

    if (level <= 2){
        return 1;
    }

    else if (level > 2 && level < 11){
        return 2;

    }
    else {
        return 3;
    }
}

void Circuit::toggleInputSignal(DraggableButton* inputButton){

    // While dense, simply flips the bool that represents the signal of the gate.
    inputButton->getGate()->setSignal(!inputButton->getGate()->getSignal());
}

void Circuit::onEvaluate(){
    bool isValidCircuit = evaluateCircuit();
    emit sendEvaluation(isValidCircuit, &currTable);
}


void Circuit::levelUp(){
    int inputs = getInputButtonCount(currentLevel);
    currTable = allTables.at(currentLevel);
    currentLevel++;
    emit sendLevel(inputs,&currTable);
}
