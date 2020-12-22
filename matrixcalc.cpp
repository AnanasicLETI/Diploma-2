#include "matrixcalc.h"

matrixCalc::matrixCalc()
{
    numDecompositionArr[0] = QVector<unsigned int>();
    numDecompositionArr[0].append(0);
    numDecompositionArr[1] = QVector<unsigned int>();
    numDecompositionArr[1].append(1);
    numDecompositionArr[2] = QVector<unsigned int>();
    numDecompositionArr[2].append(2);
    numDecompositionArr[3] = QVector<unsigned int>();
    numDecompositionArr[3].append(2);
    numDecompositionArr[3].append(1);
    numDecompositionArr[4] = QVector<unsigned int>();
    numDecompositionArr[4].append(4);
    numDecompositionArr[5] = QVector<unsigned int>();
    numDecompositionArr[5].append(4);
    numDecompositionArr[5].append(1);
    numDecompositionArr[6] = QVector<unsigned int>();
    numDecompositionArr[6].append(4);
    numDecompositionArr[6].append(2);
//    numDecompositionArr[7] = QVector<unsigned int>();
//    numDecompositionArr[7].append(7);

    numDecompositionArr[7] = QVector<unsigned int>();
    numDecompositionArr[7].append(4);
    numDecompositionArr[7].append(2);
    numDecompositionArr[7].append(1);

    numDecompositionArr[8] = QVector<unsigned int>();
    numDecompositionArr[8].append(8);
//    numDecompositionArr[8].append(1);
    numDecompositionArr[9] = QVector<unsigned int>();
    numDecompositionArr[9].append(8);
    numDecompositionArr[9].append(1);


    // Це так, не нужное
//    pseudoPrimeNum = QVector<unsigned int>();
//    pseudoPrimeNum.append(1);
//    pseudoPrimeNum.append(2);
//    pseudoPrimeNum.append(4);
//    pseudoPrimeNum.append(7);

    state = 0;
}

void matrixCalc::changeInitialData(int newRank, int newDimension)
{
    rank =newRank;
    dimension = newDimension;
}

unsigned int matrixCalc::getDimension()
{
    return dimension;
}

unsigned int matrixCalc::getRank()
{
    return rank;
}

unsigned int matrixCalc::getMaxElementValue()
{
    if(state == 0){
        return qPow(2, rank) - 1;
    }else{
        return 1;
    }

}

unsigned int matrixCalc::getInputLength()
{
    switch (state) {
    case 0:{
        return qPow(rank, dimension);
    }break;
    case 1:{
        return qPow(rank, (dimension+1));
    }break;
    case 2:{
        return qPow(rank, (dimension+1));
    }break;
    case 3:{
        return qPow(rank, (dimension+1));
    }break;
    case 4:{
        return qPow(rank, (dimension+1));
    }break;
    default:{
        return 0;
    }
    }
}

bool matrixCalc::isValidInput(QVector<unsigned int> input)
{
    for(int i=0; i<input.size();i++){
        if(input[i] > this->getMaxElementValue()){
            return false;
        }
    }
    return true;
}

void matrixCalc::setState(int newState)
{
    state = newState;
}

int matrixCalc::getState()
{
    return state;
}

QVector<QVector<unsigned int> > matrixCalc::parseInputFifth(QVector<QVector<unsigned int>> *inputVector, QString formula, int *transposeOrder)
{

    if(formula.indexOf("M") == -1){
        *transposeOrder = 0;
    }else{
        QString transNumber = formula.mid(formula.indexOf("M")+1, 1);
        *transposeOrder = transNumber.toInt();
    }
    QString mainSuperposeVectorNum = formula.mid(formula.indexOf("(q")+2, 1);


    QString tmp = formula.mid(formula.indexOf("*q")+1, (formula.indexOf(")") - formula.indexOf("*q")-1) );
    QStringList vectorNumList = tmp.split(",");

    QVector<QVector<unsigned int>> newOut;
    QVector<unsigned int> a = inputVector->at(mainSuperposeVectorNum.toInt());
    newOut.append( QVector<unsigned int>(inputVector->at( mainSuperposeVectorNum.toInt()-1 )) );

    for(int i=0; i < inputVector->size()-1; i++){
        QString num = vectorNumList[i].mid(1);
        newOut.append( QVector<unsigned int>( inputVector->at( num.toInt()-1 ) ) );
    }
    return newOut;
}

unsigned int matrixCalc::convertDataToThirdCalc(QVector<unsigned int> externalVector, QVector<QVector<unsigned int>> addedVector, QString* str)
{
    for(int i=0; i < addedVector.size();i++){
        *str += getStrFromVectorInt(externalVector);
        for(int j=i;j < addedVector.size();j++){
            *str += " x ";
            *str += getStrFromVectorInt(addedVector[j]);
        }
        *str += " = ";
        externalVector=decompose(externalVector, addedVector[i]);
    }
    *str += QString::number(externalVector[0]);
    return externalVector[0];
}

QVector<unsigned int> matrixCalc::decompose(QVector<unsigned int> externalVector, QVector<unsigned int> addedVector)
{
    int size = addedVector.size();
    int k = externalVector.size()/size;
    QVector<unsigned int> outVector;

    for(int i=0; i<k; i++){
        QVector<unsigned int> tmp = externalVector.mid(i*size, size);
        outVector.append(adding(addedVector, tmp));
    }
    return outVector;
}

unsigned int matrixCalc::adding(QVector<unsigned int> vector1, QVector<unsigned int> vector2)
{
    int size = vector1.size();
    unsigned int out;
    for(int i=0; i < size; i++){
        out = out | (vector1[i]*vector2[i]);
    }
    return out;
}

QVector<unsigned int> matrixCalc::getVectorIntFromStr(QString str)
{
    QVector<unsigned int> out;
    for(int i = 0; i < str.size(); i++){
        out.append( ((QString)str[i]).toInt() );
    }
    return out;
}

QString matrixCalc::getStrFromVectorInt(QVector<unsigned int> input)
{
    QString str;
    for(int i=0; i < input.size() ; i++){
        str += QString::number(input[i]);
    }
    return str;
}

QVector<unsigned int> matrixCalc::getVecFormFromMatrixForm(QVector<unsigned int> input)
{
    QVector<unsigned int> output;
    QVector<unsigned int> pseudoPrimeList = getPseudoPrimeVector(rank);
    int index = 0;
    while(index < input.size()){
        unsigned int out = 0;
        for(int j = 0; j < pseudoPrimeList.size(); j++){
            if( input[index] == 1){
                out += pseudoPrimeList[j];
            }
            index++;
        }
        output.append(out);
    }
    return output;
}

QVector<unsigned int> matrixCalc::getMatrixFormFromVecForm(QVector<unsigned int> input)
{
    QVector<unsigned int> output;
    QVector<unsigned int> pseudoPrimeList = getPseudoPrimeVector(rank);
    for(int i=0; i < input.size(); i++){
        for(int j = 0; j < pseudoPrimeList.size(); j++){
            if( numDecompositionArr[input[i]].contains(pseudoPrimeList[j])){
                output.append(1);
            }else{
                output.append(0);
            }
        }
    }
    return output;
}

QString matrixCalc::replaceAllVariablesInPolandParseByValues(QStringList *polandList, QVector<QVector<unsigned int> > *inputVectors)
{
    QString outStr;
    for(int i=0; i < polandList->size();i++){
        QString tmp = polandList->at(i);
        if( tmp[0] == "q"){
            QString formula = polandList->at(i);
//            outStr += "Calc: " + formula + " \n";
            QString value = getStrFromVectorInt( inputVectors->at( QString(tmp[1]).toInt() -1 ) );
            polandList->replace( i, value);
//            outStr += "\n";
        }
    }
    qDebug() << *polandList;
    return outStr;
}

QString matrixCalc::replaceAllVariablesInPolandParseByValuesButSaveFormula(QList<QPair<QString, QString> > *polandList, QVector<QVector<unsigned int> > *inputVectors)
{
    QString outStr;
    for(int i=0; i < polandList->size();i++){
        QString tmp = polandList->at(i).first;
        if( tmp[0] == "q"){
            QString formula = polandList->at(i).first;
            QString value = getStrFromVectorInt( inputVectors->at( QString(tmp[1]).toInt() -1 ) );
            polandList->replace( i, QPair<QString, QString>(value, tmp));
        }
    }
    qDebug() << *polandList;
    return outStr;
}

QString matrixCalc::getFormulaGraphTree(QStringList *polandList, int dimension)
{
    QVector<GraphTree> valueStack;

    while(!polandList->isEmpty()){
        QString tmp = polandList->takeFirst();
        if(tmp == "⋂"){
            GraphTree var_2 = valueStack.takeLast();
            GraphTree var_1 = valueStack.takeLast();
            GraphTree var_3 = var_1 + var_2;
            valueStack.append( var_3 );
            continue;
        }
        if(tmp == "*"){
            QVector<GraphTree> inputVectors;
            for (int i=0; i < dimension; i++){
                inputVectors.append( valueStack.takeLast() );
            }
            GraphTree inputMain = valueStack.takeLast() ;
            GraphTree result = inputMain.operation_Superpose(inputVectors);     //getStrFromVectorInt(getSuperpose(inputMain, inputVectors, &outStr));
            valueStack.append( result );
            continue;
        }
        if(tmp[0] == 'M'){
            GraphTree inputMain = valueStack.takeLast();
            int numOfTranpose = QString(tmp[1]).toInt();
            GraphTree result = inputMain.operation_M(numOfTranpose-1);
            valueStack.append( result );
            continue;
        }
        QStringList newValue = {tmp};
        qDebug() << newValue;
        valueStack.append( GraphTree( newValue ) );
    }
    return valueStack.last().getStringRepresentation();
}

QVector<unsigned int> matrixCalc::getSuperpose(QVector<unsigned int> inputMain, QVector<QVector<unsigned int>> inputVectors, QString *log)
{
    QString outStr;
    QVector<unsigned int> output;
    int len = this->getInputLength();
    int localLen = len/rank;

    outStr += getStrFromVectorInt(inputMain)+"*";
    for(int i = 0; i < inputVectors.size(); i++){
        outStr += getStrFromVectorInt(inputVectors[i]);
        if( i != inputVectors.size()-1){
           outStr += ",";
        }
    }
    outStr += ":\n";

    for(unsigned int i = 0; i < rank; i++){
        QVector<unsigned int> externalVector = inputMain.mid((i)*localLen, localLen);
        for(int j=0; j < localLen;j++){
            QVector<QVector<unsigned int>> add;

            for(int n=this->getDimension(); n > 0; n--){
                add.append(QVector<unsigned int>());

                for(unsigned int m=0; m< rank;m++){
                    add.last().append(inputVectors[n-1][m*localLen+j]);
                }
            }
            QString tmpStr = "";
            output.append(convertDataToThirdCalc(externalVector, add, &tmpStr));
            outStr += tmpStr + "\n";
        }
        outStr += "--------------------\n";
    }
    outStr += "S: "+ getStrFromVectorInt(output);
    if(log != nullptr ){
        *log += outStr + "\n";
    }
    return output;
}

QVector<QVector<unsigned int>> matrixCalc::getTranspose(QVector<unsigned int> input, QString *log)
{
    QVector<QVector<unsigned int>> output;
    QString outStr;
    int len = this->getInputLength();
    int n = this->getDimension();
    int k = this->getRank();
    int range;

    for(int i=1; i <= n; i++){
        range = len/qPow(k, i);
        QVector<unsigned int> tmp;
        for(int j=0;j < qPow(k, i-1);j++){ // количество частей, которые мы обкашлеваем

            int indent = (range*k)*j;
            QVector<unsigned int> tmpNum;

            for(int m=0;m < (range/k);m++){ // тут мы говорим, что для всех штук в ренже
                for(int h=0;h < k;h++){// тут определяем делимость на что мы смотрим
                    for(int l=0;l < k;l++){ // а тут мы просто берем штуку со всех ренжей
                        tmpNum.append(input[indent + l*range + m*k +h]);
                    }
                }
            }
            tmp.append(tmpNum);
        }
        if(log != nullptr){
            *log += getStrFromVectorInt( input )+":\n";
            *log += "T"+QString::number(i)+"= ";
            *log += getStrFromVectorInt(tmp)+"\n";
        }
        output.append(tmp);
    }


    return output;
}

QVector<unsigned int> matrixCalc::getIntersection(QVector<QVector<unsigned int>> input, QString *log)
{
    QVector<unsigned int> outVector = input.first();
    QString v1, v2, v3;
    QString outStr = "";

    for(int i=1; i < input.size(); i++){
        v1 = "";
        v2 = "";
        v3 = "";
        for(int j=0; j < input[i].size();j++){
            v1 += QString::number( outVector[j] );
            v2 += QString::number( input[i][j] );
            outVector[j] &= input[i][j];  // This is matrixForm and & is safe
            v3 += QString::number( outVector[j] );
        }
        outStr += v1+" ⋂\n"+v2+"\n----------\n"+v3;
    }
    *log += outStr;
    return outVector;
}

QVector<unsigned int> matrixCalc::getIntersection(QString var_1, QString var_2, QString *log)
{

    QString v1, v2, v3;
    QString outStr = "";
    v1 = var_1;
    v2 = var_2;
    QVector<unsigned int> vec_1 = getVectorIntFromStr( var_1 );
    QVector<unsigned int> vec_2 = getVectorIntFromStr( var_2 );
    QVector<unsigned int> outVector = vec_1;

    for(int i=1; i < vec_1.size(); i++){
        outVector[i] &= vec_2[i];  // This is matrixForm and & is safe
    }
    v3 = getStrFromVectorInt(outVector);

    outStr += v1+" ⋂\n"+v2+"\n----------\n"+v3;
    if(log != nullptr){
         *log += outStr;
    }

    return outVector;
}

QString matrixCalc::calcFirst(QVector<unsigned int> input)
{
    QString outStr;
    QString result;

    QVector<unsigned int> pseudoPrimeList = getPseudoPrimeVector(rank);
    QVector<QString> primeContent(pseudoPrimeList.size());

    for(int i=0; i < input.size(); i++){
        for(int j = 0; j < pseudoPrimeList.size(); j++){
            if( numDecompositionArr[input[i]].contains(pseudoPrimeList[j])){
                result += QString::number(1);
                primeContent[j] += QString::number(1);
            }else{
                result += QString::number(0);
                primeContent[j] += QString::number(0);
            }
        }
    }
    for(int i=0; i < pseudoPrimeList.size(); i++){
        outStr += "{"+ QString::number(pseudoPrimeList[i]) +"}: ";
        outStr += primeContent[i] + "\n";
    }
    outStr += "Итог: " + result;

    return outStr;
}

QString matrixCalc::calcSecond(QVector<unsigned int> input)
{
    QString outStr;

    QString result;
    QVector<unsigned int> pseudoPrimeList = getPseudoPrimeVector(rank);
    QVector<QString> primeContent(pseudoPrimeList.size());

    int index = 0;
    while(index < input.size()){
        unsigned int out = 0;
        for(int j = 0; j < pseudoPrimeList.size(); j++){
            if( input[index] == 1){
                out += pseudoPrimeList[j];
                primeContent[j].append(QString::number( pseudoPrimeList[j] ));
            }else{
                primeContent[j].append(QString::number( 0 ));
            }
            index++;
        }
        result.append(QString::number(out));
    }
    for(int i=0; i < pseudoPrimeList.size(); i++){
        outStr += "{"+ QString::number(pseudoPrimeList[i]) +"}: ";
        outStr += primeContent[i] + "\n";
    }
    outStr += "Итог: " + result;

    return outStr;
}

QString matrixCalc::calcThird(QVector<QVector<unsigned int> > input, QVector<QVector<unsigned int> > *outputData)
{
    QString outStr = "";
    int len = this->getInputLength();
    int localLen = len/rank;

    //outNotCompositeNum.append("=");
    outputData->append(QVector<unsigned int>());

    for(unsigned int i = 0; i < rank; i++){
        QVector<unsigned int> externalVector = input[0].mid((i)*localLen, localLen);
        for(int j=0; j < localLen;j++){
            QVector<QVector<unsigned int>> add;

            for(int n=this->getDimension(); n > 0; n--){
                add.append(QVector<unsigned int>());

                for(unsigned int m=0; m< this->rank;m++){
                    add.last().append(input[n][m*localLen+j]);
                }
            }
            QString tmpStr = "";
            outputData->last().append(convertDataToThirdCalc(externalVector, add, &tmpStr));
            outStr += tmpStr + "\n";
        }
        outStr += "--------------------\n";
    }
    outStr += "Итог: "+ getStrFromVectorInt(outputData->last());
    return outStr;
}

QString matrixCalc::calcFifth(QVector<QVector<unsigned int>> *inputVectors, QString formula)
{
    QString outStr;

    int dimension = inputVectors->size() - 1;

    QStringList calcList = getPolandParse(formula);
    qDebug() << "Old Calc" << calcList;
    QList<QPair<QString, QString>> newCalcList;
    for(int i=0; i < calcList.size(); i++) {
        newCalcList.append( QPair<QString, QString>(calcList[i], "") );
    }
    qDebug() << "New Calc" << newCalcList;

    outStr += replaceAllVariablesInPolandParseByValues(&calcList, inputVectors);
    outStr += replaceAllVariablesInPolandParseByValuesButSaveFormula(&newCalcList, inputVectors);

    QStringList valueStack;
    QList<QPair<QString, QString>> doubleValueStack;
    while(!newCalcList.isEmpty()){
        QPair<QString, QString> activePair = newCalcList.takeFirst();
        QString tmp = activePair.first;
        qDebug() << tmp << calcList << newCalcList << valueStack << outStr;
        if(tmp == "⋂"){
            QPair<QString, QString> term_2 = doubleValueStack.takeLast();
            QPair<QString, QString> term_1 = doubleValueStack.takeLast();

            QString var_2 = term_2.first;
            QString var_1 = term_1.first;

            QString newFormula = "("+ term_1.second +")⋂("+ term_2.second +")";
            outStr += newFormula+"\n";

            QString var_3 = getStrFromVectorInt( getIntersection(var_1, var_2) );

            QPair<QString, QString> newValue = QPair<QString, QString>(var_3, newFormula);

            outStr += var_1 +" ⋂ "+ var_2 +" = "+ var_3 +"\n";
            doubleValueStack.append(newValue);
            valueStack.append( var_3 );

            outStr += "\n";
            continue;
        }
        if(tmp == "*"){
            QVector<QVector<unsigned int>> inputVectors;
            QVector<QString> formulaVec;

            for (int i=0; i < dimension; i++){
                QPair<QString, QString> tmpPair = doubleValueStack.takeLast();
                inputVectors.append(getVectorIntFromStr( tmpPair.first ));
                formulaVec.append(tmpPair.second);
            }

            QPair<QString, QString> tmpPair = doubleValueStack.takeLast();
            QVector<unsigned int> inputMain = getVectorIntFromStr( tmpPair.first );
            QString mainFormula = tmpPair.second;

            QString newFormula = "(" + mainFormula +" * ";
            for(int i=0; i<formulaVec.size(); i++) {
                newFormula += formulaVec[i];
                if(i != formulaVec.size()-1){
                    newFormula += ",";
                }
            }newFormula += ")";
            outStr += newFormula+"\n";

            QString result = getStrFromVectorInt(getSuperpose(inputMain, inputVectors, &outStr));
            doubleValueStack.append( QPair<QString, QString>( result, newFormula) );
            valueStack.append( result );

            outStr += "\n";
            continue;
        }
        if(tmp[0] == 'M'){
            QPair<QString, QString> tmpPair = doubleValueStack.takeLast();
            QVector<unsigned int> inputMain = getVectorIntFromStr( tmpPair.first );
            int numOfTranpose = QString(tmp[1]).toInt();
            QString newFormula = "M"+ QString::number(numOfTranpose) + tmpPair.second;
            outStr += newFormula+"\n";
            QString result = getStrFromVectorInt( getTranspose(inputMain, &outStr)[ numOfTranpose-1 ] );
            doubleValueStack.append( QPair<QString, QString>( result, newFormula) );
            valueStack.append( result );

            outStr += "\n";
            continue;
        }
        doubleValueStack.append(activePair);
        valueStack.append( tmp );
    }

    QString result =  doubleValueStack.first().first;// valueStack.first();
    QString vecResult = getStrFromVectorInt( getVecFormFromMatrixForm( getVectorIntFromStr(result) ) );


    outStr += "\nРезультат в векторной форме:\n"+result+"\nРезультат в матричной форме:\n"+vecResult+"\n";
    qDebug() << "Result" << valueStack;

    return outStr;
}

QStringList matrixCalc::getPolandParse(QString formula, QString *log)
{
    QStringList outPolandParse;
    QStringList operationsStack;

    QMap<QString, int> priorityMap = {{"⋂", 0}, {"*", 1}, {"M", 2}};

    for(int i =0; i < formula.size(); i++){

        QString readingChar = QString(formula[i]);
        qDebug() << outPolandParse << operationsStack << readingChar;

        if( readingChar == "(" ){
            operationsStack.append("(");
            continue;
        }

        if( readingChar == ")"){
            while(operationsStack.last() != "("){
                outPolandParse.append( operationsStack.takeLast() );
            }
            operationsStack.removeLast();
            continue;
        }

        if(priorityMap.contains( readingChar )){
            if(readingChar == "M"){
                readingChar += QString(formula[i+1]);
                i++;
            }
            if( operationsStack.isEmpty() || operationsStack.last() == "("){
                operationsStack.append( readingChar );
                continue;
            }
            int priorityOfNewElement = priorityMap.value( readingChar );
            int priorityOfLastElement = priorityMap.value( QString(operationsStack.last()[0]) );
// M1(q1*q1)⋂(M1(M1(q2*q2)*q1)⋂M1(q1*q1))

qDebug() << priorityOfNewElement << priorityOfLastElement << operationsStack.last() << priorityMap;
            if( priorityOfNewElement <= priorityOfLastElement ){
                outPolandParse.append( operationsStack.takeLast() );
            }
            operationsStack.append(readingChar);
            continue;
        }

        if(readingChar == "q"){
            readingChar += QString(formula[i+1]);
            i++;
            outPolandParse.append( readingChar );
            continue;
        }

        qDebug() << "POLAND PARSE ERROR";

    }
    for (int i =0; i < operationsStack.size(); i++) {
        outPolandParse.append( operationsStack.takeFirst() );
    }
    return outPolandParse;
}

QString matrixCalc::getFirstFormulaSectionEntry(QString formula)
{
    QString sectionExpression = "((M[\\d]+)?\\(q[\\d]\\*(q[\\d]\\,)*q[\\d]\\))";
    QRegExp rx(sectionExpression);
    int pos = rx.indexIn(formula);
    qDebug() << "getFirstFormulaSectionEntry" << pos;
    return rx.capturedTexts()[0];
}

QVector<unsigned int> matrixCalc::calcFifthSection(QVector<QVector<unsigned int> > *inputVectors, QString formula, QString *log)
{
    int transposeNumber;

    QVector<QVector<unsigned int>> parsedVector = parseInputFifth(inputVectors, formula, &transposeNumber);
    QVector<unsigned int> mainVector = parsedVector.takeFirst();
    QVector<QVector<unsigned int>> vectors = parsedVector;

    QString outStr;
    qDebug() << "calcFifth" << mainVector << vectors;
    QVector<unsigned int> result;
    result = getSuperpose(mainVector, vectors, &outStr);
    outStr += "\n";

    if(transposeNumber != 0){
        QVector<QVector<unsigned int>> tranposeResult = getTranspose(result);
        result = tranposeResult[transposeNumber-1];
        outStr += "T: " + getStrFromVectorInt(result);
        outStr += "\n";
    }
    if(log != nullptr){
        *log += outStr;
    }
    return result;
}

QVector<unsigned int> matrixCalc::getPseudoPrimeVector(unsigned int rank)
{
    QVector<unsigned int> out;
    unsigned int pseudoPrime = 0;
    for(unsigned int i=0; i < rank; i++){
        pseudoPrime ++;
        while(numDecompositionArr[pseudoPrime].size() != 1){
            pseudoPrime ++;
            if(pseudoPrime > pseudoMathNumRange){
                emit sendError("Ошибка преобразования вектора, выход за диапазон");
                return QVector<unsigned int>();
            }
        }
        out.append(pseudoPrime);
    }
    return out;
}


