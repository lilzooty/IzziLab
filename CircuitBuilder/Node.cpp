#include "Node.h"


Node::Node(GateType type): input1(nullptr), input2(nullptr), output(nullptr){}


bool getValue(){
    return value;
}


bool Node::isFullyConnnected(){

    // Inverters only need to have 1 input and 1 output
    if(gateType == INVERTER ){
        if (input1 != nullptr || input != nullptr ){
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
            value = input1.getValue() && input2.getValue();
            break;

        case OR_GATE:
            value = input1.getValue() || input2.getValue();

        case INVERTER:
            if(input1 != nullptr){
                value = !input1.getValue();
            }
            else{
                value = !input2.getValue();
            }
            break;

        default:
            return false;
            break;

    }

    return true;


}


