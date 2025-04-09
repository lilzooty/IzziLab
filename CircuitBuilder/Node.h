#ifndef NODE_H
#define NODE_H
#include <string>
enum GateType {
    INPUT,
    OUTPUT,
    OR_GATE,
    INVERTER,
    AND_GATE,
    NOR_GATE,
    NAND_GATE,
    XOR_GATE,
    XNOR_GATE
};

class Node
{
public:


    Node(GateType type);

    /**
     * @brief connnectNode Adds param node n as the output.
     * @return
     */
    // bool connnectNode(Node n);

    // bool deleteInput1();
    // bool deleteInput2();
    bool getSignal();

    /**
     * @brief evaluate Converts the input to a value based on the gate type.
     */
    bool evaluate();


    /**
     * @return True if node has 2 inputs and an output (unless its an inverter)
     */
    bool isFullyConnnected();

    bool getValue();



private:

    GateType gateType;
    Node* input1;
    Node* input2;
    Node* output;
    bool signal;

};

#endif // NODE_H
