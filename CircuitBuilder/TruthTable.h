#ifndef TRUTHTABLE_H
#define TRUTHTABLE_H

#include <QVector>
#include <QPair>
#include <QList>
#include <QString>


class TruthTable{

public:
    TruthTable(const QVector<QVector<int>>& inputs, const QVector<int>& outputs);

    QList<QPair<QVector<int>, int>> getRows() const;

    QString getHint();

    void setHint(QString levelHint);

private:
    QList<QPair<QVector<int>, int>> tableRows;

    QString hint;


};

#endif //
