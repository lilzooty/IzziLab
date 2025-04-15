#include "Node.h"


Node::Node(GateType type): gateType(type), input1(nullptr), input2(nullptr), output(nullptr), signal(false){}

bool Node::connnectNode(Node n){

    // Prevent adding to occupied nodes
    if (n.availableInputs() > 0  && this->output == nullptr){
       // this->output = n;
        return true;
    }

    return false;

}

void Node::deleteInput1(){
    input1->output = nullptr;
    this->input1 = nullptr;
}

void Node::deleteInput2(){
    input2->output = nullptr;
    this->input2 = nullptr;
}

bool Node::hasOutput(){
    return output != nullptr;
}

bool Node::evaluate() {
    // Validate required inputs
    if ((gateType == OUTPUT || gateType == INVERTER) && !input1) {
        return false;
    }
    else if (gateType != INPUT &&
             gateType != OUTPUT &&
             gateType != INVERTER) {
        if (!input1 || !input2) {
            return false; // Missing one or both inputs
        }
    }

    //Get inputs signal is not null.
     bool in1 = input1 ? input1->getSignal() : false;
     bool in2 = input2? input2->getSignal() : false;

    // Calculate output based on gate type
    switch (gateType) {
    case OUTPUT:
        signal = in1;
        break;

    case INVERTER:
        signal = !in1;
        break;

    case AND_GATE:
        signal = in1 && in2;
        break;

    case OR_GATE:
        signal = in1 || in2;
        break;

    case NAND_GATE:
        signal = !(in1 && in2);
        break;

    case NOR_GATE:
        signal = !(in1 || in2);
        break;

    case XOR_GATE:
        signal = in1 != in2;
        break;

    case XNOR_GATE:
        signal = in1 == in2;
        break;

    default:
        return false;
    }

    return true;
}

int Node::availableInputs(){
    int inputs = 0;

    if (input1 != nullptr){
        inputs ++;
    }

    if (input2 != nullptr){
        inputs += 2;
    }

    return inputs;
}

bool Node::addInput(Node* n, int input){

    // Reject improper input calls
    if (input > 2 || input < 1){
        return false;
    }

    if(!n || n == this){
        return false;
    }

    if (input == 1 && input1 == nullptr){
            input1 = n;
        n->addOutput(this);
            return true;
        }

    else if (input == 2 && input2 == nullptr) {
        input2 = n;
        n->addOutput(this);
        return true;
}
    return false;
}

bool Node::getSignal(){
    return this->signal;
}

void Node::setSignal(int signal){
    this->signal = signal;
}

GateType Node::getGateType(){
    return gateType;
}

bool Node::checkValidConnection(Node* target, int inputSlot) const {
    if(!target || target == this){
        return false;
    }
    if(target->getGateType() == INPUT){
        return false;
    }
    if(gateType == OUTPUT){
        return false;
    }
    if(inputSlot == 1){
        return target->getInput1() == nullptr;
    }
    if(inputSlot == 2){
        return target->getInput2() == nullptr;
    }
    return true;
}

bool Node::hasOneInput() const {
    return gateType == INVERTER || gateType == OUTPUT;
}


Node* Node::getInput1(){
    return input1;
}

Node* Node::getInput2(){
    return input2;
}

void Node::setInput1(Node* node){
    input1 = node;
}

void Node::setInput2(Node* node){
    input2 = node;
}

QVector<Node*> Node::getOutputs(){
    return outputs;
}

void Node::addOutput(Node* outputNode) {
    if (outputNode && !outputs.contains(outputNode)) {
        outputs.append(outputNode);
    }
}

void Node::deleteOutputs() {
    // Disconnect from all output nodes
    for (Node* output : outputs) {
        output->removeInput(this);
    }
    outputs.clear();
}

void Node::removeInput(Node* input) {
    if (input1 == input) {
        input1 = nullptr;
    }
    if (input2 == input) {
        input2 = nullptr;
    }
}

void Node::removeOutput(Node* outputNode){
    if(!outputNode){
        return;
    }
    outputs.removeOne(outputNode);
}

void Node::removeAllInputs() {
    if (input1) {
        input1->removeOutput(this);
        input1 = nullptr;
    }
    if (input2) {
        input2->removeOutput(this);
        input2 = nullptr;
    }
}

void Node::reset() {
    signal = false;
    for (Node* output : outputs) {
        output->reset();
    }
}

void Node::disconnectAll() {
    for (Node* out : outputs) {
        out->removeInput(this);
    }
    outputs.clear();

    if (input1) input1->removeOutput(this);
    if (input2) input2->removeOutput(this);
    input1 = input2 = nullptr;
}
