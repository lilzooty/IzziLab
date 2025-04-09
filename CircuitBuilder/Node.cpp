#include "Node.h"


Node::Node(GateType type): gateType(type), input1(nullptr), input2(nullptr), output(nullptr), signal(false){}


bool Node::getSignal(){
    return this->signal;
}


bool Node::isFullyConnnected(){

    // Inverters only need to have 1 input and 1 output
    if(gateType == INVERTER ){
        if (input1 != nullptr || input2 != nullptr ){
            if (output != nullptr){
                return true;
            }

        }
        return false;
    }

    // Other gates require 2 inputs and 1 ouput to be fully connected.
    if (input1 != nullptr && input2 != nullptr){

        if (output != nullptr){
            return true;
        }
    }

    return false;

}


bool Node::evaluate(){

    if (!isFullyConnnected()){
        return false;
    }

    // TODO finish eval for other gate types
    switch(gateType){

    case AND_GATE:
        signal = input1->getSignal() & input2->getSignal();
        break;

    case OR_GATE:
        signal = input1->getSignal() | input2->getSignal();

    case INVERTER:
        if(input1 != nullptr){
            signal = !input1->getSignal();
        }
        else{
            signal = !input2->getSignal();
        }
        break;
    case NOR_GATE:
        signal = !(input1->getSignal() | input2->getSignal());
        break;
    case NAND_GATE:
        signal = !(input1->getSignal() & input2->getSignal());
        break;
    case XOR_GATE:
        signal = (input1->getSignal() ^ input2->getSignal());
        break;
    case XNOR_GATE:
        signal = !(input1->getSignal() ^ input2->getSignal());
        break;

    default:
        return false;
        break;

    }

    return true;


}


