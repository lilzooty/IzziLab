#ifndef NODE_H
#define NODE_H
#include <string>

class Node
{
public:
    enum NodeType {
        INPUT,
        OUTPUT,
        ORGATE,
        ANDGATE,
        NORGATE,
        NANDGATE,
        XORGATE,
        XNORGATE
    }
    Node();

    bool addNode(Node n);

    bool deleteInput1();
    bool deleteInput2();

    /**
     * @brief isFullyConnnected
     * @return True if node has 2 inputs and an output;
     */
    bool isFullyConnnected();



private:

    NodeType nodeType;
    Node input1;
    Node input2;
    Node output;
    bool value;

};

#endif // NODE_H
