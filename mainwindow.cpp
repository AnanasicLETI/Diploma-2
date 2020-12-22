#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    matrixCalcEntity = new matrixCalc();
    QObject::connect(matrixCalcEntity, SIGNAL(sendError(QString)), this, SLOT(errorMesage(QString)));
    matrixCalcEntity->changeInitialData(ui->RankSpinBox->value(), ui->dimensionSpinBox->value());
    matrixCalcEntity->setState(ui->tabWidget->currentIndex());

    isValidInput = false;
//    outputData = new QVector<QVector<unsigned int>>();

    updateInformation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::errorMesage(QString str)
{
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}

void MainWindow::on_RankSpinBox_valueChanged(int Rank)
{
    Q_UNUSED(Rank);
    updateInformation();
}

void MainWindow::on_dimensionSpinBox_valueChanged(int dimension)
{
    Q_UNUSED(dimension);
    updateInformation();


}

void MainWindow::on_caclFirstBtn_clicked()
{

    if(!isValidInput){
        QMessageBox msgBox;
        msgBox.setText("Некорректный ввод");
        msgBox.exec();
        return;
    }

    QString outStr;

    switch (matrixCalcEntity->getState()) {
    case 0:{
        outStr = matrixCalcEntity->calcFirst(getVectorIntFromStr(ui->input1->toPlainText()));
    }break;
    case 1:{
        outStr = matrixCalcEntity->calcSecond(getVectorIntFromStr(ui->input2->toPlainText()));
    }break;
    case 2:{
        QVector<QVector<unsigned int>> input;
        QString strExternal = ui->input3_External->text();
        QString strInternal = ui->input3_Internal->toPlainText();
        input.append(getVectorIntFromStr(strExternal));
        QList<QString> listStrInternal = strInternal.split('\n');
        for(int i=0; i < listStrInternal.size();i++){
            input.append(getVectorIntFromStr(listStrInternal[i]));
        }
        matrixCalcEntity->getSuperpose(input[0], input.mid(1), &outStr);
    }break;
    case 3:{
        QVector<unsigned int> input = getVectorIntFromStr(ui->input4->toPlainText());
        matrixCalcEntity->getTranspose(input, &outStr);
    }break;
    case 4:{
        QVector<QVector<unsigned int>> input;

        for(int i=0; i < lineEditList.size() ; i++){
            if( ui->vectorInputRadioBtn->isChecked() ){
                input.append( matrixCalcEntity->getMatrixFormFromVecForm( getVectorIntFromStr(lineEditList[i]->text()) ));
            }else{
                input.append( getVectorIntFromStr(lineEditList[i]->text()) );
            }

        }
        outStr = matrixCalcEntity->calcFifth(&input, ui->regExLineEdit->text());
        QStringList tmp = matrixCalcEntity->getPolandParse( ui->regExLineEdit->text() );
        saveLogFile("graph", matrixCalcEntity->getFormulaGraphTree( &tmp, ui->dimensionSpinBox->value() ));
    }break;
    }

    ui->outputPlainTextEdit->setPlainText(outStr);

}



void MainWindow::updateInformation()
{
    matrixCalcEntity->changeInitialData(ui->RankSpinBox->value(), ui->dimensionSpinBox->value());

    unsigned int requiredInputLength = matrixCalcEntity->getInputLength();
    unsigned int maxElementValue = matrixCalcEntity->getMaxElementValue();
    int state = matrixCalcEntity->getState();

    QString errorStr = inputValidation(state);

    switch (state) {
    case 0:{
        ui->input1CountLbl->setText("Вводимая длина: "+QString::number(ui->input1->toPlainText().size()));
    }break;
    case 1:{
        ui->input2CountLbl->setText("Вводимая длина: "+QString::number(ui->input2->toPlainText().size()));
    }break;
    case 2:{
        ui->input3CountLbl->setText("Длина внешней последовательности: "+QString::number(ui->input3_External->text().size()));
    }break;
    case 3:{
        ui->input4CountLbl->setText("Вводимая длина: "+QString::number(ui->input4->toPlainText().size()));
    }break;
    case 4:{
        if( ui->vectorInputRadioBtn->isChecked() ){
            maxElementValue= qPow(2, ui->RankSpinBox->value()) - 1;
            requiredInputLength  = qPow(ui->RankSpinBox->value(), (ui->dimensionSpinBox->value()));
        }
    }break;
    }
    ui->statusbar->showMessage("Необходимая длина: "+ QString::number(requiredInputLength)
            +" Максимальный элемент: "+QString::number(maxElementValue)
            +"| "+errorStr);
}

QVector<unsigned int> MainWindow::getVectorIntFromStr(QString str)
{
    QVector<unsigned int> out;
    for(int i = 0; i < str.size(); i++){
        out.append( ((QString)str[i]).toInt() );
    }
    return out;
}

QString MainWindow::getStrFromVectorInt(QVector<unsigned int> input)
{
    QString str;
    for(int i=0; i < input.size() ; i++){
        str += QString::number(input[i]);
    }
    return str;
}


QString MainWindow::inputValidation(int state)
{
    QString errorStr;
    switch (state) {
    case 0:{
        isValidInput = firstValidation(&errorStr);
    }break;
    case 1:{
        isValidInput = secondValidation(&errorStr);
    }break;
    case 2:{
        isValidInput = thirdValidation(&errorStr);
    }break;
    case 3:{
        isValidInput = fouthValidation(&errorStr);
    }break;
    case 4:{
        isValidInput = fifthValidation(&errorStr);
    }break;
    }

    return errorStr;
}

bool MainWindow::firstValidation(QString* errorStr)
{
    unsigned short err = simpleValidate(ui->input1->toPlainText());
    *errorStr = errorValidateDecode(err);
    if(err){
        ui->input1->setStyleSheet("background-color: #FF6859;");
        return false;
    }else{
        ui->input1->setStyleSheet("");
        *errorStr = "";
        return true;
    }

}

bool MainWindow::secondValidation(QString* errorStr)
{
    unsigned short err = simpleValidate(ui->input2->toPlainText());
    *errorStr = errorValidateDecode(err);
    if(err){
        ui->input2->setStyleSheet("background-color: #FF6859;");
        return false;
    }else{
        ui->input2->setStyleSheet("");
        *errorStr = "";
        return true;
    }

    return true;
}

bool MainWindow::thirdValidation(QString *errorStr)
{

    bool lenErrorExt = false;
    bool digitalErrorExt = false;
    bool maxValueErrorExt = false;

    bool lenErrorInter = false;
    bool digitalErrorInter = false;
    bool maxValueErrorInter = false;

    QString strExternal = ui->input3_External->text();

    if( ((unsigned int)strExternal.size()) != matrixCalcEntity->getInputLength()){
        lenErrorExt = true;
    }
    if(!isDigitalString(strExternal)){
        digitalErrorExt = true;
    }
    if(!matrixCalcEntity->isValidInput(getVectorIntFromStr(strExternal))){
        maxValueErrorExt = true;
    }


    QString strInternal = ui->input3_Internal->toPlainText();
    QList<QString> strListInternal = strInternal.split('\n');
    for(int i=0; i < strListInternal.size(); i++){
        if((unsigned int)strListInternal[i].size() != matrixCalcEntity->getInputLength()){
            lenErrorInter = true;
        }
        if(!isDigitalString(strListInternal[i])){
            digitalErrorInter = true;
        }
        if(!matrixCalcEntity->isValidInput(getVectorIntFromStr(strListInternal[i]))){
            maxValueErrorInter = true;
        }
    }
    if(lenErrorExt || lenErrorInter){
        *errorStr += " ошибка длины строки";
    }
    if(digitalErrorExt || digitalErrorInter){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " допустим ввод только цифр";
    }
    if(maxValueErrorExt || maxValueErrorInter){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " превышение значений";
    }

    if(lenErrorExt || digitalErrorExt || maxValueErrorExt){
        ui->input3_External->setStyleSheet("background-color: #FF6859;");
    }else{
        ui->input3_External->setStyleSheet("");
    }

    if(lenErrorInter || digitalErrorInter || maxValueErrorInter){
        ui->input3_Internal->setStyleSheet("background-color: #FF6859;");
    }else{
        ui->input3_Internal->setStyleSheet("");
    }

    if(lenErrorExt || lenErrorInter || digitalErrorExt || digitalErrorInter || maxValueErrorExt || maxValueErrorInter){
        return false;
    }


    if((unsigned int)strListInternal.size() != matrixCalcEntity->getDimension()){
        *errorStr = " неверное количество внутренних последовательностей";
        ui->input3_Internal->setStyleSheet("background-color: #FF6859;");
        return false;
    }
    return true;
}

bool MainWindow::fouthValidation(QString *errorStr)
{
    unsigned short err = simpleValidate(ui->input4->toPlainText());
    *errorStr = errorValidateDecode(err);
    if(err){
        ui->input4->setStyleSheet("background-color: #FF6859;");
        return false;
    }else{
        ui->input4->setStyleSheet("");
        *errorStr = "";
        return true;
    }
}

bool MainWindow::fifthValidation(QString *errorStr)
{
    bool isValidRegEx = false;
    bool isValidEditLines = false;

    if( validateFormulaByRegEx(ui->dimensionSpinBox->value()) ){
        ui->regExLineEdit->setStyleSheet("");
        isValidRegEx = true;
    }else{
        ui->regExLineEdit->setStyleSheet("background-color: #FF6859;");
    }
    updateLineEditList();
    *errorStr = validateAllLineEdit();
    if(*errorStr == ""){
        isValidEditLines = true;
    }
    qDebug() << "return isValid" << isValidEditLines << isValidRegEx;
    return (isValidRegEx & isValidEditLines);
}

void MainWindow::updateLineEditList()
{
    QLayoutItem* item;
    int count =  ui->dimensionSpinBox->value() - ui->sequenceLayout_4->count() + 1;

    if(ui->tabWidget->currentIndex() != 4){
        return;
    }

    while ( ( count < 0 ) && ((item = ui->sequenceLayout_4->takeAt( ui->sequenceLayout_4->count()-1 )) != NULL) ){
        QLayoutItem* item2;
        if( item->layout() != NULL ){
            while ( (item2 = item->layout()->takeAt(0)) != NULL )
            {
                delete item2->widget();
                delete item2;
            }
            lineEditList.removeLast();
        }
        delete item->widget();
        delete item;
        count ++;
    }

    for(int i=0; i < count ; i++){
        QHBoxLayout *newHBox = new QHBoxLayout();
        QLabel *newLable = new QLabel("q"+QString::number( ui->sequenceLayout_4->count()+1 ));
        QLineEdit *newLineEdit = new QLineEdit();
        newHBox->addWidget(newLable);
        newHBox->addWidget(newLineEdit);

        QObject::connect(newLineEdit, SIGNAL( textChanged(QString) ), this, SLOT( updateInformation(QString) ));

        lineEditList.append(newLineEdit);

        ui->sequenceLayout_4->addLayout(newHBox);
    }
}



void MainWindow::on_regExLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateInformation();
}

bool MainWindow::validateFormulaByRegEx(int dimension)
{

    QString dimensionStr = QString::number(dimension);
    QString dimensionStrMinusOne = QString::number(dimension - 1);
    QString dimensionStrPlusOne = QString::number(dimension + 1);

    QString sectionExpression = "(M[1-"+dimensionStr+"])?"
          "\\(q[1-"+dimensionStrPlusOne+"]\\*"
          "(q[1-"+dimensionStrPlusOne+"]\\,){"+ dimensionStrMinusOne +"}"
          "q[1-"+dimensionStrPlusOne+"]\\)";

    QString sectionFull = "(M[1-"+dimensionStr+"])?"
          "\\((q[1-"+dimensionStrPlusOne+"]|"+sectionExpression+")\\*"
          "((q[1-"+dimensionStrPlusOne+"]|"+sectionExpression+")\\,){"+ dimensionStrMinusOne +"}"
          "(q[1-"+dimensionStrPlusOne+"]|"+sectionExpression+")\\)";

    QRegExp rx("^\\(*"+sectionFull+"\\)*(⋂\\(*"+sectionFull+"\\)*)*");
    QRegExpValidator v(rx, 0);
    int pos = 0;
    QString s = ui->regExLineEdit->text();
    if( v.validate(s, pos) == QValidator::Acceptable ){
        if(s.count("(") != s.count(")")){
            return false;
        }
        return true;
    }
    return false;
}

QString MainWindow::validateAllLineEdit()
{
    unsigned short error = 0;
    QString errorStr;

    for(int i=0; i < lineEditList.size() ; i++){

        unsigned short localError = 0;
        QVector<unsigned int> input = getVectorIntFromStr( lineEditList[i]->text() );

        if( ui->vectorInputRadioBtn->isChecked() ){
            unsigned int vectorMaxValue = qPow(2, ui->RankSpinBox->value()) - 1;
            for(int i=0; i<input.size();i++){
                if(input[i] > vectorMaxValue ){
                    localError = 1;
                    break;
                }
            }
            input = matrixCalcEntity->getMatrixFormFromVecForm(input);
        }
        localError |= simpleValidate( getStrFromVectorInt( input ) );
        error = error | localError;
        if(localError == 0){
            lineEditList[i]->setStyleSheet("");
        }else{
            lineEditList[i]->setStyleSheet("background-color: #FF6859;");
        }
    }
    errorStr += errorValidateDecode(error);
    return errorStr;
}

unsigned short MainWindow::simpleValidate(QString inputStr)
{
    unsigned short error = 0;


    if( ((unsigned int)inputStr.size()) != matrixCalcEntity->getInputLength()){
        error |= 1;
    }

    error = (error << 1);
    if(!isDigitalString(inputStr)){
        error |= 1;
    }

    error = (error << 1);
    if(!matrixCalcEntity->isValidInput(getVectorIntFromStr(inputStr))){
        error |= 1;
    }

    return error;

}

QString MainWindow::errorValidateDecode(unsigned short error)
{
    QString outStr;
    if(error % 2){
        outStr += " превышение значений";
    }
    if((error >> 1) % 2){
        if( !(outStr).isEmpty())
            outStr +=",";
        outStr += " допустим ввод только цифр";
    }
    if((error >> 2) % 2){
        if( !(outStr).isEmpty())
            outStr +=",";
        outStr += " ошибка длины строки";
    }


    return outStr;
}

void MainWindow::saveLogFile(QString prefix, QString data)
{
    QFile file(prefix+"_"+ QDateTime::currentDateTime().toString("MM_dd_hh_mm_ss") +".txt");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-16");
    stream << data;
    stream.flush();
    file.close();
}

bool MainWindow::isDigitalString(QString str)
{
    for(int i = 0; i < str.size(); i++){
        if(!str[i].isDigit()){
            return false;
        }
    }
    return true;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    matrixCalcEntity->setState(index);
    ui->tabWidget->setCurrentIndex(index);
    updateInformation();
}

void MainWindow::on_saveBtn_clicked()
{
    if(ui->outputPlainTextEdit->toPlainText().isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Некорректный ввод");
        msgBox.exec();
        return;
    }

    QString outStr;

    switch (matrixCalcEntity->getState()) {
    case 0:{
        outStr = "Ввод:\n" + ui->input1->toPlainText() + "\n";
    }break;
    case 1:{
        outStr = "Ввод:\n" + ui->input2->toPlainText() + "\n";
    }break;
    case 2:{
        outStr = "Ввод:\nВнешняя последовательность:\n" + ui->input3_External->text() + "\n";
        outStr += "Внутренняя последовательность:\n" +ui->input3_Internal->toPlainText() + "\n";
    }break;
    case 3:{
        outStr = "Ввод:\n" + ui->input4->toPlainText() + "\n";
    }break;
    }
    outStr += "Вывод:\n"+ui->outputPlainTextEdit->toPlainText();

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранение файла", "", "*.txt");

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            errorMesage("Неверное имя файла");
        } else {
            QTextStream stream(&file);
            stream << outStr;
            stream.flush();
            file.close();
        }
    }
}

void MainWindow::on_vectorInputRadioBtn_clicked()
{
    updateInformation();
}


void MainWindow::on_input1_textChanged()
{
    updateInformation();
}

void MainWindow::on_input2_textChanged()
{
    updateInformation();
}

void MainWindow::on_input3_External_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateInformation();
}

void MainWindow::on_input3_Internal_textChanged()
{
    updateInformation();
}

void MainWindow::on_input4_textChanged()
{
    updateInformation();
}

void MainWindow::on_matrixInputRadioBtn_clicked()
{
    updateInformation();
}

void MainWindow::updateInformation(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateInformation();
}

void MainWindow::on_pushButton_clicked()
{
    QString magigChar = "⋂";
    ui->regExLineEdit->setText(ui->regExLineEdit->text() + magigChar);
}

void MainWindow::on_helpAction_triggered()
{
    QString help = "Для работы программы пользователю необходимо:\n\
    1)Заполнить поле «Размерность\n\
    2)Заполнить поле «Ранг»\n\
    3)Выбрать способ ввода последовательностей в полях «Выбор способа ввода»\n\
    4)Ввести последовательности q (1…n) в соответствующие поля (поля добавляются автоматически в зависимости от значения в поле «Размерность»)\n\
    5)Ввести выражение, которое необходимо вычислить в поле Терм 't='\n\
    6)Нажать «>» Решение с ответом отобразится в поле «Результат»\n\n\
Дерево c последовательностью выбора операций сохраняется в файл формата .txt\n\
Символы вводимые пользователем в поле терм «t = » :\n\
    * - умножение, необходимое для операции «Нахождение матрицы суперпозиции мультиопераций»\n\
    M(1…n) – транспонирование матрицы по i-элементу\n\
    ⋂ - пересечение, для удобства кнопка расположена под полем «Терм»\n\
    () – скобки, применяются для обозначения приоритета операций\n\
Пример ввода терма: (M1(q1*q2,q2)⋂M2(q2*q3,q1))⋂(q1*(q2*q1,q3),q1)\n";
      QMessageBox msgBox;
      msgBox.setText(help);
      msgBox.setWindowTitle("Help");
      msgBox.exec();
}
