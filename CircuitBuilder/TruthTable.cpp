#include "TruthTable.h"

TruthTable::TruthTable(const QVector<QVector<int>>& inputs, const QVector<int>& outputs, QString hint, QString levelDescription){
    for (int i = 0; i < inputs.size(); ++i) {
        tableRows.append(qMakePair(inputs[i], outputs[i]));
    }
    this->hint = hint;
    this->levelDescription = levelDescription;
}

TruthTable::TruthTable(){
    hint = "";
}

QList<QPair<QVector<int>, int>> TruthTable::getRows() const {
    return tableRows;
}

void TruthTable::setHint(QString levelHint){
    hint = levelHint;
}

QString TruthTable::getHint() const{
    return hint;
}

QString TruthTable::getDescription(){
    return levelDescription;
}


