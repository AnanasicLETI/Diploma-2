#ifndef MATRIXCALC_H
#define MATRIXCALC_H

#include <QObject>
#include <QLineEdit>
#include <QPair>

#include <QRegExp>

#include <QVector>
#include <qmath.h>

#include <QDebug>

#include "graphtree.h"

class matrixCalc: public QObject
{
    Q_OBJECT

public:
    matrixCalc();
    void changeInitialData(int Rank, int dimension);

    unsigned int getDimension();
    unsigned int getRank();

    unsigned int getMaxElementValue();
    unsigned int getInputLength();

    bool isValidInput(QVector<unsigned int>);

    void setState(int newState);
    int getState();

    QVector<QVector<unsigned int>> parseInputFifth(QVector<QVector<unsigned int>> *inputVector, QString formula, int *transposeOrder);

    unsigned int convertDataToThirdCalc(QVector<unsigned int> externalVector, QVector<QVector<unsigned int>> addedVector, QString* str);
    QVector<unsigned int> decompose(QVector<unsigned int> externalVector, QVector<unsigned int> addedVector);
    unsigned int adding(QVector<unsigned int> vector1, QVector<unsigned int> vector2);

    QVector<unsigned int> getVectorIntFromStr(QString str);
    QString getStrFromVectorInt(QVector<unsigned int> input);

    QVector<unsigned int> getVecFormFromMatrixForm(QVector<unsigned int>);
    QVector<unsigned int> getMatrixFormFromVecForm(QVector<unsigned int>);

    QString replaceAllVariablesInPolandParseByValues(QStringList *polandList, QVector<QVector<unsigned int> > *inputVectors);
    QString replaceAllVariablesInPolandParseByValuesButSaveFormula(QList<QPair<QString, QString>> *polandList, QVector<QVector<unsigned int> > *inputVectors);
    QString getFormulaGraphTree(QStringList *polandList, int dimension);
//    QStringList getListOfVariablesFromPolandParse();

    QVector<unsigned int> getSuperpose(QVector<unsigned int>, QVector<QVector<unsigned int>>, QString *log = nullptr);
    QVector<QVector<unsigned int>> getTranspose(QVector<unsigned int> input, QString *log = nullptr);

    QVector<unsigned int> getIntersection(QVector<QVector<unsigned int>> input, QString *log);
    QVector<unsigned int> getIntersection(QString var_1, QString var_2, QString *log = nullptr);

    QString calcFirst(QVector<unsigned int> input);
    QString calcSecond(QVector<unsigned int> input);
    QString calcThird(QVector<QVector<unsigned int>> input, QVector<QVector<unsigned int> > *outputData);
    QString calcFourth (QVector<unsigned int> input, QVector<QVector<unsigned int>>* outputData);
    QString calcFifth(QVector<QVector<unsigned int>> *inputVectors, QString formula);

    // QStringList
    QStringList getPolandParse(QString formula, QString *log = nullptr);
    QString getFirstFormulaSectionEntry(QString formula);

    QVector<unsigned int> calcFifthSection(QVector<QVector<unsigned int>> *inputVectors, QString formula, QString *log = nullptr);

//    QString getStrFromVectorUInt(QVector<unsigned int> vec);

private:

    QVector<unsigned int> getPseudoPrimeVector(unsigned int rank);


    static const unsigned int pseudoMathNumRange = 10;

    QVector<unsigned int> numDecompositionArr[pseudoMathNumRange];
//    QVector<unsigned int> pseudoPrimeNum;

    unsigned int rank;
    unsigned int dimension;

    int state;

signals:
    void sendError(QString);

};


#endif // MATRIXCALC_H
