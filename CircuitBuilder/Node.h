#ifndef NODE_H
#define NODE_H

#include <QVector>

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

// NOTE: currently, the Node class will not override existing node connections. maybe it should?
class Node
{
public:


    Node(GateType type);

    /**
     * @brief connnectNode Adds param node n as the output Node.
     * @return
     */

    bool connnectNode(Node n);

    /**
     * @brief deleteInput1 Sets current node's input 1 to nullptr
     * @return
     */
    void deleteInput1();

    void deleteInput2();

    void setInput1(Node* node);

    void setInput2(Node* node);

    bool getSignal();

    void setSignal(int signal);

    GateType getGateType();

    Node* getInput1();

    Node* getInput2();

    QVector<Node*> getOutputs();

    void deleteOutputs();

    void addOutput(Node* output);

    void removeOutput(Node* output);

    void removeInput(Node* input);

    void removeAllInputs();

    void reset();

    void disconnectAll();




    /**
     * @return True if node has an output, false otherwise.
     */
    bool hasOutput();

    /**
     * @brief evaluate Converts the input to a value based on the gate type.
     */
    bool evaluate();

    /**
     * Returns 0 if no inputs are available.
     * returns 1 if input 1 is available.
     * returns 2 if input 2 is available.
     * returns 3 if bothInputs are available.
     * @return
     */
    int availableInputs();

    /**
     * @brief addInput
     * @param input Either 1 or 2 representing which input is being added to.
     * @return True if sucessfully added, false if adding input failed.
     */
    bool addInput(Node* n, int input);

private:

    GateType gateType;
    Node* input1;
    Node* input2;
    Node* output;
    bool signal;

    bool checkValidConnection(Node* input, int inputSlot) const;

    bool hasOneInput() const;

    QVector<Node*> outputs;
};

#endif // NODE_H
