#include "Gate.h"


Gate::Gate(GateType type): gateType(type), input1(nullptr), input2(nullptr), output(nullptr), signal(false){}

Gate::Gate(){}

bool Gate::connnectNode(Gate g){

    // Prevent adding to occupied nodes
    if (g.availableInputs() > 0  && this->output == nullptr){
       // this->output = n;
        return true;
    }

    return false;
}

void Gate::deleteInput1(){
    if (input1) {
        input1->removeOutput(this);
        input1 = nullptr;
    }
}

void Gate::deleteInput2(){
    if (input2) {
        input2->removeOutput(this);
        input2 = nullptr;
    }
}

bool Gate::hasOutput(){
    return !outputs.isEmpty();
}

bool Gate::evaluate() {
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

int Gate::availableInputs(){
    int inputs = 0;

    if (input1 != nullptr){
        inputs ++;
    }

    if (input2 != nullptr){
        inputs += 2;
    }

    return inputs;
}

bool Gate::addInput(Gate* g, int input){
    if (!g || g == this || input < 1 || input > 2) return false;

    if (input == 1 && !input1) {
        input1 = g;
        g->addOutput(this);
        return true;
    }
    else if (input == 2 && !input2) {
        input2 = g;
        g->addOutput(this);
        return true;
    }
    return false;
}

bool Gate::getSignal(){
    return this->signal;
}

GateType Gate::getGateType(){
    return gateType;
}

Gate* Gate::getInput1(){
    return input1;
}

Gate* Gate::getInput2(){
    return input2;
}


void Gate::setSignal(bool signal){
    this->signal = signal;
}

void Gate::setInput1(Gate* gate){
    input1 = gate;
}

void Gate::setInput2(Gate* gate){
    input2 = gate;
}

bool Gate::checkValidConnection(Gate* target, int inputSlot) const {
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

bool Gate::hasOneInput() const {
    return gateType == INVERTER || gateType == OUTPUT;
}

QVector<Gate*> Gate::getOutputs(){
    return outputs;
}

void Gate::addOutput(Gate* outputGate) {
    if (outputGate && !outputs.contains(outputGate)) {
        outputs.append(outputGate);
    }
}

void Gate::deleteOutputs() {
    // Disconnect from all output nodes
    for (Gate* output : outputs) {
        output->removeInput(this);
    }
    outputs.clear();
}

void Gate::removeInput(Gate* input) {
    if (input1 == input) {
        input1 = nullptr;
    }
    if (input2 == input) {
        input2 = nullptr;
    }
}

void Gate::removeOutput(Gate* outputGate){
    if(!outputGate){
        return;
    }
    outputs.removeOne(outputGate);
}

void Gate::removeAllInputs() {
    if (input1) {
        input1->removeOutput(this);
        input1 = nullptr;
    }
    if (input2) {
        input2->removeOutput(this);
        input2 = nullptr;
    }
}

void Gate::reset() {
    signal = false;
}

void Gate::disconnectAll() {
    deleteOutputs();
    removeAllInputs();
}
