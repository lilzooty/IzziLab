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

class Gate
{
public:

    Gate();

    Gate(GateType type);

    void removeOutput(Gate* output); // one of these has to be redundant

    void removeInput(Gate* input);

    bool getSignal();

    void setSignal(bool signal);

    GateType getGateType();

    Gate* getInput1();

    Gate* getInput2();

    void reset();

    void disconnectAll();

    /**
     * @brief evaluate Converts the input to a value based on the gate type.
     */
    bool evaluate();


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

    void addOutput(Gate* output);

    void deleteOutputs();

    void removeAllInputs();

    QVector<Gate*> outputs;

};

#endif // GATE_H
