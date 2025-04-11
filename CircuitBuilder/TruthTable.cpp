#include "TruthTable.h"

TruthTable::TruthTable(const QVector<QVector<int>>& inputs, const QVector<int>& outputs){
    for (int i = 0; i < inputs.size(); ++i) {
        tableRows.append(qMakePair(inputs[i], outputs[i]));
    }
}

QList<QPair<QVector<int>, int>> TruthTable::getRows() const {
    return tableRows;
}

void TruthTable::setHint(QString levelHint){
    hint = levelHint;
}

QString TruthTable::getHint(){
    return hint;
}

