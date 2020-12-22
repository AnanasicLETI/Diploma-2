#ifndef GRAPHTREE_H
#define GRAPHTREE_H

#include <QString>
#include <QStringList>

#include <QDebug>


class GraphTree
{
public:
    GraphTree(QString formula);
    GraphTree(QStringList newRepresentation);
    int getWidth();
    int getHeigth();

    GraphTree operator+(GraphTree adding);
    GraphTree operation_Superpose(QVector<GraphTree> adding);
    GraphTree operation_M(int index);
    QString operator[](int i);

    QString getStringRepresentation();

private:
    QStringList representation;
};

#endif // GRAPHTREE_H
