#ifndef GATE_H
#define GATE_H

#include <QVector>

/**
 * @brief enum for different types of gates
 */
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

/**
 * @brief The Gate class represents a digital logic gate. This class contains
 * data for the gates inputs, outputs, and its signal.
 */
class Gate
{
public:

    /**
     * @brief default constructor for gate with no type
     */
    Gate();

    /**
     * @brief  constructor for gate with specified gatetype
     * @param type of the gate
     */
    Gate(GateType type);

    /**
     * @brief removeOutput removes gate from list of outputs
     * @param output the gate output to remove
     */
    void removeOutput(Gate* output);

    /**
     * @brief removes input parameter gate from this gate
     * @param input the input gate to remove
     */
    void removeInput(Gate* input);

    /**
     * @brief gets the signal of this gate
     * @return the signal of this gate(true or false)
     */
    bool getSignal();

    /**
     * @brief updates the signal of this gate
     * @param signal the signal to update
     */
    void setSignal(bool signal);

    /**
     * @brief getGateType
     * @return the type of this gate
     */
    GateType getGateType();

    /**
     * @brief gets the first input connection
     * @return pointer to the first input gate
     */
    Gate* getInput1();

    /**
     * @brief gets the second input connection
     * @return pointer to the second input gate
     */
    Gate* getInput2();

    /**
     * @brief sets the signal of this gate to false
     */
    void reset();

    /**
     * @brief disconnects this gate from any gate it is connected to
     */
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


    bool hasOneInput() const;

    void addOutput(Gate* output);

    void deleteOutputs();

    void removeAllInputs();

    QVector<Gate*> outputs;

};

#endif
