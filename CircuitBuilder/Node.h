#ifndef NODE_H
#define NODE_H
#include <string>

class Node
{
public:
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

    std::string gateType;
    Node input1;
    Node input2;

    bool value;

};

#endif // NODE_H
