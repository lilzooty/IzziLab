#ifndef TRUTHTABLE_H
#define TRUTHTABLE_H

#include <QVector>
#include <QPair>
#include <QList>
#include <QString>

/**
 * @brief The TruthTable class represents a truth table for a circuit. It shows
 * how a circuit's outputs should respond to various combinations of 1's and 0's.
 * This truth table also contains data for a hint to how it should be solved as well
 * as a levelDescription for introductory circuits.
 */
class TruthTable{

public:
    /**
     * @brief Default constructor for empty table
     */
    TruthTable();

    /**
     * @brief creates a truth table with given inputs, outputs, hint, and levelDescription.
     * @param inputs a vector of rows of inputs
     * @param outputs a vector of output corresponding to inputs
     * @param hint a string that gives explanation of how to create expected circuit
     * @param levelDescription the descriptoin of how the circuit works(for intro levels)
     */
    TruthTable(const QVector<QVector<int>>& inputs, const QVector<int>& outputs, QString hint, QString levelDescription);

    /**
     * @brief gets the rows of inputs and their expected output
     * @return a list of input output pairs
     */
    QList<QPair<QVector<int>, int>> getRows() const;

    /**
     * @brief gets the tables hint
     * @return the table hint
     */
    QString getHint() const;

    /**
     * @brief gets the tables circuit description
     * @return the circuit description
     */
    QString getDescription();

private:
    /**
     * @brief the list of inputs and their expected outputs
     */
    QList<QPair<QVector<int>, int>> tableRows;

    /**
     * @brief the hint for the game
     */
    QString hint;

    /**
     * @brief the description for the circuit given to users on intro levels
     */
    QString levelDescription;
};

#endif
