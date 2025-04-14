#include "Circuit.h"

Circuit::Circuit(QObject *parent) : QObject{parent}, gates{} {

}

void Circuit::initializeEasyTruthTables(){
    // Lvl 1: input → output
    QVector<QVector<int>> input1 = { {0}, {1} };

    QVector<int> output1 = { 0, 1 };
    TruthTable t1(input1, output1);
    easyTables.append(t1);
  //  t1.setHint();

    // Lvl 2: inverter (NOT gate)
    QVector<int> output2 = { 1, 0 };
    easyTables.append(TruthTable(input1, output2));
}

void Circuit::initializeMedTruthTables() {
    QVector<QVector<int>> inputs2 = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };

    // Lvl 1: AND gate
    QVector<int> outputAND = {0, 0, 0, 1};
    medTables.append(TruthTable(inputs2, outputAND));

    // Lvl 2: OR gate
    QVector<int> outputOR = {0, 1, 1, 1};
    medTables.append(TruthTable(inputs2, outputOR));

    // Lvl 3: NAND gate
    QVector<int> outputNAND = {1, 1, 1, 0};
    medTables.append(TruthTable(inputs2, outputNAND));

    // Lvl 4: NOR gate
    QVector<int> outputNOR = {1, 0, 0, 0};
    medTables.append(TruthTable(inputs2, outputNOR));

    // Lvl 5: XOR gate
    QVector<int> outputXOR = {0, 1, 1, 0};
    medTables.append(TruthTable(inputs2, outputXOR));

    // Lvl 6: XNOR gate
    QVector<int> outputXNOR = {1, 0, 0, 1};
    medTables.append(TruthTable(inputs2, outputXNOR));

    // Medium hard problems

    // Lvl 7: A AND NOT B
    QVector<int> outputAND_NOT = {0, 0, 1, 0};
    medTables.append(TruthTable(inputs2, outputAND_NOT));

    // Lvl 8: NOT A OR B
    QVector<int> outputNOTA_OR_B = {1, 1, 0, 1};
    medTables.append(TruthTable(inputs2, outputNOTA_OR_B));

    // Lvl 9: A XOR B AND A
    QVector<int> outputXOR_AND_A = {0, 0, 1, 0};
    medTables.append(TruthTable(inputs2, outputXOR_AND_A));
}

void Circuit::initializeHardTruthTables(){
    QVector<QVector<int>> inputs3 = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        {0, 1, 1},
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 0},
        {1, 1, 1}
    };

    // Lvl 1: A AND B OR C
    QVector<int> output_AND_OR = {0, 1, 0, 1, 1, 1, 1, 1};
    hardTables.append(TruthTable(inputs3, output_AND_OR));

    // Lvl 2: NOT (A OR B) AND C
    QVector<int> output_NOTOR_AND = {0, 1, 0, 0, 0, 0, 0, 0};
    hardTables.append(TruthTable(inputs3, output_NOTOR_AND));

    // Lvl 3: A XOR B XOR C
    QVector<int> output_3XOR = {0, 1, 1, 0, 1, 0, 0, 1};
    hardTables.append(TruthTable(inputs3, output_3XOR));

    // Lvl 4: A AND B XOR B AND C
    QVector<int> output_AND_XOR_AND = {0, 0, 0, 1, 0, 1, 1, 0};
    hardTables.append(TruthTable(inputs3, output_AND_XOR_AND));

    // Lvl 5: NOT (A AND B OR NOT C)
    QVector<int> output_NOT_COMBO = {1, 1, 1, 1, 1, 1, 0, 0};
    hardTables.append(TruthTable(inputs3, output_NOT_COMBO));

    // Lvl 6: A OR NOT B AND B OR NOT C
    QVector<int> output_OR_NOT_AND = {1, 1, 1, 1, 1, 1, 1, 1};
    hardTables.append(TruthTable(inputs3, output_OR_NOT_AND));

    // Lvl 7: Majority function (at least 2 inputs are 1)
    QVector<int> output_MAJORITY = {0, 0, 0, 1, 0, 1, 1, 1};
    hardTables.append(TruthTable(inputs3, output_MAJORITY));

    // Lvl 8: A NAND B NOR B NAND C
    QVector<int> output_NAND_NOR_NAND = {0, 0, 0, 0, 0, 0, 0, 1};
    hardTables.append(TruthTable(inputs3, output_NAND_NOR_NAND));
}

void Circuit::addNode(GateType gate) {
    gates.push_back(Node(gate));
}

void Circuit::updateButton(DraggableButton *button) {
    if (mostRecentButton != nullptr) {
        emit mostRecentButtonUpdated(mostRecentButton);
    }
    mostRecentButton = button;
}
