// #include <QCoreApplication>
// #include <QDebug>
// #include "Node.h"
// #include "TruthTable.h"
// #include "Circuit.h"

// // Test 1: Direct input-to-output connection
// bool testDirectConnection() {
//     // Create input and output nodes
//     Node input(INPUT);
//     Node output(OUTPUT);


//     // Truth table: input -> output (same signal)
//     TruthTable table(
//         {{0},{1}},  // Input combinations
//         {0, 1}       // Expected outputs
//         );

//     Circuit circuit;
//     circuit.onConnectNode(&input, &output, 1);
//     bool result = circuit.evaluateCircuit({&input}, &output, table);

//     qDebug() << "Direct connection test:" << (result ? "PASSED" : "FAILED");
//     return result;
// }

// // Test 2: Simple AND gate configuration
// bool testAndGate() {
//     Node input1(INPUT);
//     Node input2(INPUT);
//     Node andGate(AND_GATE);

//     TruthTable table(
//         {{0,0}, {0,1}, {1,0}, {1,1}},
//         {0, 0, 0, 1}
//         );

//     Circuit circuit;
//     circuit.onConnectNode(&input1, &andGate, 1);
//     circuit.onConnectNode(&input2, &andGate, 2);
//     bool result = circuit.evaluateCircuit({&input1, &input2}, &andGate, table);

//     qDebug() << "AND gate test:" << (result ? "PASSED" : "FAILED");
//     return result;
// }

// // Test 3: Inverter (NOT gate) configuration
// bool testInverter() {
//     Node input(INPUT);
//     Node inverter(INVERTER);

//     TruthTable table(
//         {{0}, {1}},
//         {1, 0}
//         );

//     Circuit circuit;
//     circuit.onConnectNode(&input, &inverter, 1);
//     bool result = circuit.evaluateCircuit({&input}, &inverter, table);

//     qDebug() << "Inverter test:" << (result ? "PASSED" : "FAILED");
//     return result;
// }

// // Test 4: Complex circuit (NAND implementation)
// bool testNandCircuit() {
//     Node input1(INPUT);
//     Node input2(INPUT);
//     Node andGate(AND_GATE);
//     Node inverter(INVERTER);

//     TruthTable table(
//         {{0,0}, {0,1}, {1,0}, {1,1}},
//         {1, 1, 1, 0}  // NAND truth table
//         );

//     Circuit circuit;
//     circuit.onConnectNode(&input1, &andGate, 1);
//     circuit.onConnectNode(&input2, &andGate, 2);
//     circuit.onConnectNode(&andGate, &inverter, 1);
//     bool result = circuit.evaluateCircuit({&input1, &input2}, &inverter, table);

//     qDebug() << "NAND circuit test:" << (result ? "PASSED" : "FAILED");
//     return result;
// }

// // Test 5: Incomplete circuit detection
// bool testIncompleteCircuit() {
//     Node input(INPUT);
//     Node andGate(AND_GATE);

//     TruthTable table(
//         {{0}, {1}},  // Invalid input count but should fail earlier
//         {0, 0}
//         );

//     Circuit circuit;
//     circuit.onConnectNode(&input, &andGate, 1);
//     bool result = circuit.evaluateCircuit({&input}, &andGate, table);

//     // We expect this to fail due to incomplete connections
//     qDebug() << "Incomplete circuit test:" << (!result ? "PASSED" : "FAILED");
//     return !result;
// }

// // Test 6: Multi-level circuit (XOR implementation)
// bool testXorCircuit() {
//     Node input1(INPUT);
//     Node input2(INPUT);

//     Node and1(AND_GATE);
//     Node and2(AND_GATE);
//     Node orGate(OR_GATE);
//     Node inverter1(INVERTER);
//     Node inverter2(INVERTER);

//     TruthTable table(
//         {{0,0}, {0,1}, {1,0}, {1,1}},
//         {0, 1, 1, 0}  // XOR truth table
//         );

//     Circuit circuit;
//     circuit.onConnectNode(&input1, &inverter1, 1);
//     circuit.onConnectNode(&input2, &inverter2, 1);

//     circuit.onConnectNode(&input1, &and1, 1);
//     circuit.onConnectNode(&inverter2, &and1, 2);

//     circuit.onConnectNode(&inverter1, &and2, 1);
//     circuit.onConnectNode(&input2, &and2, 2);

//     circuit.onConnectNode(&and1, &orGate, 1);
//     circuit.onConnectNode(&and2, &orGate, 2);


//     bool result = circuit.evaluateCircuit({&input1, &input2}, &orGate, table);

//     qDebug() << "XOR circuit test:" << (result ? "PASSED" : "FAILED");
//     return result;
// }

// // Test 7: Disconnecting nodes
// bool testDisconnection() {
//     Node input1(INPUT);
//     Node input2(INPUT);
//     Node andGate(AND_GATE);
//     Node output(OUTPUT);

//     Circuit circuit;
//     circuit.onConnectNode(&input1, &andGate, 1);
//     circuit.onConnectNode(&input2, &andGate, 2);
//     circuit.onConnectNode(&andGate, &output, 1);

//     // Verify initial connection works
//     TruthTable validTable(
//         {{0,0}, {0,1}, {1,0}, {1,1}},
//         {0, 0, 0, 1}
//         );
//     bool initialResult = circuit.evaluateCircuit({&input1, &input2}, &output, validTable);

//     // Disconnect one input
//     circuit.onDisconnectNode(&input1, &andGate);

//     // Create new truth table expecting failure
//     TruthTable invalidTable(
//         {{0,0}, {0,1}, {1,0}, {1,1}},
//         {0, 0, 0, 0}  // All false due to incomplete circuit
//         );

//     bool disconnectResult = !circuit.evaluateCircuit({&input1, &input2}, &output, invalidTable);

//     qDebug() << "Disconnection test:" << (initialResult && disconnectResult ? "PASSED" : "FAILED");
//     return initialResult && disconnectResult;
// }

// // Test 8: Deleting nodes
// bool testNodeDeletion() {
//     Node* input1 = new Node(INPUT);
//     Node* input2 = new Node(INPUT);
//     Node* andGate = new Node(AND_GATE);
//     Node* output = new Node(OUTPUT);

//     Circuit circuit;
//     circuit.onConnectNode(input1, andGate, 1);
//     circuit.onConnectNode(input2, andGate, 2);
//     circuit.onConnectNode(andGate, output, 1);

//     // Delete the AND gate
//     circuit.onDeleteNode(andGate);

//     // Attempt evaluation - should fail
//     TruthTable table(
//         {{0,0}, {0,1}, {1,0}, {1,1}},
//         {0, 0, 0, 0}
//         );

//     bool result = !circuit.evaluateCircuit({input1, input2}, output, table);

//     qDebug() << "Node deletion test:" << (result ? "PASSED" : "FAILED");
//     return result;
// }

// // Test 9: Reconnecting nodes
// bool testReconnection() {
//     Node input1(INPUT);
//     Node input2(INPUT);
//     Node andGate(AND_GATE);
//     Node orGate(OR_GATE);
//     Node output(OUTPUT);

//     Circuit circuit;

//     // Initial configuration: input1 -> andGate, input2 -> andGate, andGate -> output
//     circuit.onConnectNode(&input1, &andGate, 1);
//     circuit.onConnectNode(&input2, &andGate, 2);
//     circuit.onConnectNode(&andGate, &output, 1);

//     // Verify initial configuration works (AND gate)
//     TruthTable andTable(
//         {{0,0}, {0,1}, {1,0}, {1,1}},
//         {0, 0, 0, 1}
//         );
//     bool initialResult = circuit.evaluateCircuit({&input1, &input2}, &output, andTable);

//     // Disconnect input2 from andGate
//     circuit.onDisconnectNode(&input2, &andGate);

//     // Connect input2 to orGate
//     circuit.onConnectNode(&input2, &orGate, 1);

//     // Disconnect andGate from output
//     circuit.onDisconnectNode(&andGate, &output);

//     // Reconnect input1 to orGate (2nd input)
//     circuit.onConnectNode(&input1, &orGate, 2);

//     // Connect orGate to output
//     circuit.onConnectNode(&orGate, &output, 1);

//     // Verify new configuration works (OR gate)
//     TruthTable orTable(
//         {{0,0}, {0,1}, {1,0}, {1,1}},
//         {0, 1, 1, 1}
//         );
//     bool reconnectionResult = circuit.evaluateCircuit({&input1, &input2}, &output, orTable);

//     qDebug() << "Reconnection test:" << (initialResult && reconnectionResult ? "PASSED" : "FAILED");
//     return initialResult && reconnectionResult;
// }

// // Test 10: Cycle detection
// bool testCycleDetection() {
//     Node* input = new Node(INPUT);
//     Node* and1 = new Node(AND_GATE);
//     Node* and2 = new Node(AND_GATE);
//     Node* output = new Node(OUTPUT);

//     Circuit circuit;
//     circuit.registerNode(input);
//     circuit.registerNode(and1);
//     circuit.registerNode(and2);
//     circuit.registerNode(output);

//     // Create a legitimate circuit first
//     circuit.onConnectNode(input, and1, 1);
//     circuit.onConnectNode(input, and2, 1);
//     circuit.onConnectNode(and1, and2, 2);
//     circuit.onConnectNode(and2, output, 1);

//     // Circuit should be acyclic at this point
//     bool noCycleResult = circuit.isAcyclic(output);

//     // Create a cycle: and2 -> and1 (making and1 <-> and2)
//     circuit.onConnectNode(and2, and1, 2);

//     // Circuit should now have a cycle
//     bool cycleResult = !circuit.isAcyclic(output);

//     qDebug() << "Cycle detection test:" << (noCycleResult && cycleResult ? "PASSED" : "FAILED");

//     // Clean up
//     delete input;
//     delete and1;
//     delete and2;
//     delete output;

//     return noCycleResult && cycleResult;
// }


// int main(int argc, char *argv[]) {
//     QCoreApplication a(argc, argv);

//     bool allTestsPassed = true;

//     allTestsPassed &= testDirectConnection();
//     allTestsPassed &= testAndGate();
//     allTestsPassed &= testInverter();
//     allTestsPassed &= testNandCircuit();
//     allTestsPassed &= testIncompleteCircuit();
//     allTestsPassed &= testXorCircuit();
//     allTestsPassed &= testDisconnection();
//     allTestsPassed &= testNodeDeletion();
//     allTestsPassed &= testReconnection();
//     allTestsPassed &= testCycleDetection();

//     qDebug() << "\n=== All tests " << (allTestsPassed ? "PASSED ===" : "FAILED ===");

//     return allTestsPassed ? 0 : 1;
// }
#include <QCoreApplication>
#include <QDebug>
#include "Node.h"
#include "Circuit.h"
#include "TruthTable.h"

// Test 0: Empty circuit evaluation
bool testEmptyEvaluation() {
    Circuit circuit;
    bool result = !circuit.evaluateCircuit();  // Expect failure
    qDebug() << "Empty circuit evaluation test:" << (result ? "PASSED" : "FAILED");
    return result;
}

// Test 1: Direct connection (input → output)
bool testDirectConnection() {
    Node* input = new Node(INPUT);
    Node* output = new Node(OUTPUT);
    TruthTable table({{0}, {1}}, {0, 1});

    Circuit circuit;
    circuit.registerNode(input);
    circuit.registerNode(output);
    circuit.onConnectNode(input, output, 1);
    circuit.setTruthTable(table);

    bool result = circuit.evaluateCircuit();
    qDebug() << "Direct connection test:" << (result ? "PASSED" : "FAILED");

    delete input;
    delete output;
    return result;
}

// Test 2: AND gate
bool testAndGate() {
    Node* input1 = new Node(INPUT);
    Node* input2 = new Node(INPUT);
    Node* andGate = new Node(AND_GATE);
    Node* output = new Node(OUTPUT);
    TruthTable table({{0,0}, {0,1}, {1,0}, {1,1}}, {0,0,0,1});

    Circuit circuit;
    circuit.registerNode(input1);
    circuit.registerNode(input2);
    circuit.registerNode(andGate);
    circuit.registerNode(output);
    circuit.onConnectNode(input1, andGate, 1);
    circuit.onConnectNode(input2, andGate, 2);
    circuit.onConnectNode(andGate, output, 1);
    circuit.setTruthTable(table);

    bool result = circuit.evaluateCircuit();
    qDebug() << "AND gate test:" << (result ? "PASSED" : "FAILED");

    delete input1;
    delete input2;
    delete andGate;
    delete output;
    return result;
}

// Test 3: Inverter (NOT gate)
bool testInverter() {
    Node* input = new Node(INPUT);
    Node* notGate = new Node(INVERTER);
    Node* output = new Node(OUTPUT);
    TruthTable table({{0}, {1}}, {1, 0});

    Circuit circuit;
    circuit.registerNode(input);
    circuit.registerNode(notGate);
    circuit.registerNode(output);
    circuit.onConnectNode(input, notGate, 1);
    circuit.onConnectNode(notGate, output, 1);
    circuit.setTruthTable(table);

    bool result = circuit.evaluateCircuit();
    qDebug() << "Inverter test:" << (result ? "PASSED" : "FAILED");

    delete input;
    delete notGate;
    delete output;
    return result;
}

// Test 4: NAND using AND + NOT
bool testNandCircuit() {
    Node* input1 = new Node(INPUT);
    Node* input2 = new Node(INPUT);
    Node* andGate = new Node(AND_GATE);
    Node* notGate = new Node(INVERTER);
    Node* output = new Node(OUTPUT);
    TruthTable table({{0,0},{0,1},{1,0},{1,1}}, {1,1,1,0});

    Circuit circuit;
    circuit.registerNode(input1);
    circuit.registerNode(input2);
    circuit.registerNode(andGate);
    circuit.registerNode(notGate);
    circuit.registerNode(output);
    circuit.onConnectNode(input1, andGate, 1);
    circuit.onConnectNode(input2, andGate, 2);
    circuit.onConnectNode(andGate, notGate, 1);
    circuit.onConnectNode(notGate, output, 1);
    circuit.setTruthTable(table);

    bool result = circuit.evaluateCircuit();
    qDebug() << "NAND circuit test:" << (result ? "PASSED" : "FAILED");

    delete input1;
    delete input2;
    delete andGate;
    delete notGate;
    delete output;
    return result;
}

// Test 5: Incomplete gate (only one input to AND)
bool testIncompleteCircuit() {
    Node* input = new Node(INPUT);
    Node* andGate = new Node(AND_GATE);
    Node* output = new Node(OUTPUT);
    TruthTable table({{0}, {1}}, {0, 0});  // Input size mismatch to simulate failure

    Circuit circuit;
    circuit.registerNode(input);
    circuit.registerNode(andGate);
    circuit.registerNode(output);
    circuit.onConnectNode(input, andGate, 1);  // only 1 input connected
    circuit.onConnectNode(andGate, output, 1);
    circuit.setTruthTable(table);

    bool result = !circuit.evaluateCircuit();  // Should fail
    qDebug() << "Incomplete circuit test:" << (result ? "PASSED" : "FAILED");

    delete input;
    delete andGate;
    delete output;
    return result;
}

// Test 6: XOR gate via multi-level (A AND NOT B) OR (NOT A AND B)
bool testXorCircuit() {
    Node* A = new Node(INPUT);
    Node* B = new Node(INPUT);
    Node* notA = new Node(INVERTER);
    Node* notB = new Node(INVERTER);
    Node* and1 = new Node(AND_GATE);
    Node* and2 = new Node(AND_GATE);
    Node* orGate = new Node(OR_GATE);
    Node* output = new Node(OUTPUT);
    TruthTable table({{0,0},{0,1},{1,0},{1,1}}, {0,1,1,0});

    Circuit circuit;
    circuit.registerNode(A);
    circuit.registerNode(B);
    circuit.registerNode(notA);
    circuit.registerNode(notB);
    circuit.registerNode(and1);
    circuit.registerNode(and2);
    circuit.registerNode(orGate);
    circuit.registerNode(output);

    circuit.onConnectNode(A, notA, 1);
    circuit.onConnectNode(B, notB, 1);

    circuit.onConnectNode(A, and2, 1);
    circuit.onConnectNode(notB, and2, 2);

    circuit.onConnectNode(notA, and1, 1);
    circuit.onConnectNode(B, and1, 2);

    circuit.onConnectNode(and1, orGate, 1);
    circuit.onConnectNode(and2, orGate, 2);

    circuit.onConnectNode(orGate, output, 1);
    circuit.setTruthTable(table);

    bool result = circuit.evaluateCircuit();
    qDebug() << "XOR gate test:" << (result ? "PASSED" : "FAILED");

    delete A; delete B;
    delete notA; delete notB;
    delete and1; delete and2;
    delete orGate; delete output;
    return result;
}


// Test 10: Cycle detection
bool testCycleDetection() {
    Node* A = new Node(AND_GATE);
    Node* B = new Node(AND_GATE);
    Node* input = new Node(INPUT);
    Node* output = new Node(OUTPUT);

    Circuit circuit;
    circuit.registerNode(input);
    circuit.registerNode(A);
    circuit.registerNode(B);
    circuit.registerNode(output);

    circuit.onConnectNode(input, A, 1);
    circuit.onConnectNode(A, B, 1);
    circuit.onConnectNode(B, A, 2); // cycle here
    circuit.onConnectNode(B, output, 1);

    circuit.setTable("medium", 0); // Medium AND gate table

    bool result = !circuit.evaluateCircuit(); // Should fail due to cycle
    qDebug() << "Cycle detection test:" << (result ? "PASSED" : "FAILED");

    delete A; delete B; delete input; delete output;
    return result;
}

bool testHardModeMajorityFunction() {
    // Inputs: A, B, C
    Node* A = new Node(INPUT);
    Node* B = new Node(INPUT);
    Node* C = new Node(INPUT);

    // Create 3 AND gates to compute pairwise (A&B), (A&C), (B&C)
    Node* andAB = new Node(AND_GATE);
    Node* andAC = new Node(AND_GATE);
    Node* andBC = new Node(AND_GATE);

    // OR gate to combine the outputs of the three AND gates
    Node* or1 = new Node(OR_GATE);
    Node* output = new Node(OUTPUT);

    Circuit circuit;
    circuit.registerNode(A);
    circuit.registerNode(B);
    circuit.registerNode(C);
    circuit.registerNode(andAB);
    circuit.registerNode(andAC);
    circuit.registerNode(andBC);
    circuit.registerNode(or1);
    circuit.registerNode(output);

    // Connect A & B to AND1
    circuit.onConnectNode(A, andAB, 1);
    circuit.onConnectNode(B, andAB, 2);

    // Connect A & C to AND2
    circuit.onConnectNode(A, andAC, 1);
    circuit.onConnectNode(C, andAC, 2);

    // Connect B & C to AND3
    circuit.onConnectNode(B, andBC, 1);
    circuit.onConnectNode(C, andBC, 2);

    // Connect AND1 & AND2 to OR gate
    circuit.onConnectNode(andAB, or1, 1);
    circuit.onConnectNode(andAC, or1, 2);

    // Use another OR to combine OR1 and AND3 (if you want to expand to 2-level OR)
    // Here we just feed OR1 into OUTPUT, and skip one AND.
    // But for better accuracy, do one more OR
    Node* or2 = new Node(OR_GATE);
    circuit.registerNode(or2);

    circuit.onConnectNode(or1, or2, 1);
    circuit.onConnectNode(andBC, or2, 2);

    circuit.onConnectNode(or2, output, 1);

    // Set the truth table to hard mode level 6 (index 6 = majority logic)
    circuit.setTable("hard", 6);

    // Evaluate
    bool result = circuit.evaluateCircuit();
    qDebug() << "Hard mode level 6 (majority function) test:" << (result ? "PASSED" : "FAILED");

    delete A; delete B; delete C;
    delete andAB; delete andAC; delete andBC;
    delete or1; delete or2;
    delete output;

    return result;
}

// Entry point
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    bool allTestsPassed = true;

    allTestsPassed &= testEmptyEvaluation();
    allTestsPassed &= testDirectConnection();
    allTestsPassed &= testAndGate();
    allTestsPassed &= testInverter();
    allTestsPassed &= testNandCircuit();
    allTestsPassed &= testIncompleteCircuit();
    allTestsPassed &= testXorCircuit();

    // allTestsPassed &= testUnusedInput();
    allTestsPassed &= testCycleDetection();
    allTestsPassed &= testHardModeMajorityFunction();

    qDebug() << "\n=== All tests" << (allTestsPassed ? "PASSED ===" : "FAILED ===");
    return allTestsPassed ? 0 : 1;
}
