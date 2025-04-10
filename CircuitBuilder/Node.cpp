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

bool Node::getSignal(){
    return this->signal;
}

bool Node::hasOutput(){
    return output != nullptr;
}

bool Node::evaluate(){

   // if (!isFullyConnnected()){
        return false;
 //   }

    // TODO finish eval for other gate types
    switch(gateType){

    case AND_GATE:
        signal = input1->getSignal() && input2->getSignal();
        break;

    case OR_GATE:
        signal = input1->getSignal() || input2->getSignal();

    case INVERTER:
        if(input1 != nullptr){
            signal = !input1->getSignal();
        }
        else{
            signal = !input2->getSignal();
        }
        break;
    case NOR_GATE:
        signal = !(input1->getSignal() || input2->getSignal());
        break;
    case NAND_GATE:
        signal = !(input1->getSignal() && input2->getSignal());
        break;
    case XOR_GATE:
        signal = ((input1->getSignal() || input2->getSignal()) && !(input1->getSignal() && input2->getSignal()));
        break;
    case XNOR_GATE:
        signal = ((input1->getSignal() && input2->getSignal()) || (!input1->getSignal() && !input2->getSignal()));
        break;

    default:
        return false;
        break;
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

bool Node::addInput(Node n, int input){

    // Reject improper input calls
    if (input > 2 || input < 0){
        return false;
    }

    if (n.output != nullptr){
        return false;
    }

    if (input == 1){
        if (input1 != nullptr){
           // input1 = n;
            n.output = this;
            return true;
        }
    }

    if (input == 2){
        if (input2 != nullptr){
          //  input2 = n;
            n.output = this;
            return true;
        }
    }

    return false;
}
