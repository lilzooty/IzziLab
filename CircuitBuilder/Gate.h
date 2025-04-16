#ifndef GATE_H
#define GATE_H

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
class Gate
{
public:

    Gate();


    Gate(GateType type);

    /**
     * @brief connnectNode Adds param node n as the output Node.
     * @return
     */

    bool connnectNode(Gate g);

    /**
     * @brief deleteInput1 Sets current node's input 1 to nullptr
     * @return
     */
    void deleteInput1();

    void deleteInput2();

    void removeOutput(Gate* output); // one of these has to be redundant

    void removeInput(Gate* input);


    void setInput1(Gate* gate);

    void setInput2(Gate* gate);

    bool getSignal();

    void setSignal(int signal);

    GateType getGateType();

    Gate* getInput1();

    Gate* getInput2();

    QVector<Gate*> getOutputs();

    void deleteOutputs();

    void addOutput(Gate* output);


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
    bool addInput(Gate* n, int input);

private:

    GateType gateType;
    Gate* input1;
    Gate* input2;
    Gate* output;
    bool signal;

    bool checkValidConnection(Gate* input, int inputSlot) const;

    bool hasOneInput() const;

    QVector<Gate*> outputs;
};

#endif // GATE_H
