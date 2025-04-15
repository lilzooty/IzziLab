#include "Gate.h"


Gate::Gate(GateType type): gateType(type), input1(nullptr), input2(nullptr), output(nullptr), signal(false){}

bool Gate::connnectNode(Gate g){

    // Prevent adding to occupied nodes
    if (g.availableInputs() > 0  && this->output == nullptr){
       // this->output = n;
        return true;
    }

    return false;
}

void Gate::deleteInput1(){
    input1->output = nullptr;
    this->input1 = nullptr;
}

void Gate::deleteInput2(){
    input2->output = nullptr;
    this->input2 = nullptr;
}

bool Gate::hasOutput(){
    return output != nullptr;
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

    // Reject improper input calls
    if (input > 2 || input < 1){
        return false;
    }

    if(!g || g == this){
        return false;
    }

    if (input == 1 && input1 == nullptr){
            input1 = g;
        g->addOutput(this);
            return true;
        }

    else if (input == 2 && input2 == nullptr) {
        input2 = g;
        g->addOutput(this);
        return true;
}
    return false;
}

bool Gate::getSignal(){
    return this->signal;
}

void Gate::setSignal(int signal){
    this->signal = signal;
}

GateType Gate::getGateType(){
    return gateType;
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


Gate* Gate::getInput1(){
    return input1;
}

Gate* Gate::getInput2(){
    return input2;
}

void Gate::setInput1(Gate* gate){
    input1 = gate;
}

void Gate::setInput2(Gate* gate){
    input2 = gate;
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
    for (Gate* output : outputs) {
        output->reset();
    }
}

void Gate::disconnectAll() {
    for (Gate* out : outputs) {
        out->removeInput(this);
    }
    outputs.clear();

    if (input1) input1->removeOutput(this);
    if (input2) input2->removeOutput(this);
    input1 = input2 = nullptr;
}
