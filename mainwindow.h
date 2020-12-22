#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QFile>

#include <QDateTime>

#include <QRegExp>
#include <QRegExpValidator>

#include "matrixcalc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void errorMesage(QString);

private slots:
    void on_RankSpinBox_valueChanged(int arg1);

    void on_dimensionSpinBox_valueChanged(int arg1);

    void on_caclFirstBtn_clicked();

    void on_input1_textChanged();

    void on_tabWidget_tabBarClicked(int index);

    void on_input2_textChanged();

    void on_input3_External_textChanged(const QString &arg1);

    void on_input3_Internal_textChanged();

    void on_input4_textChanged();

    void on_saveBtn_clicked();

    void on_regExLineEdit_textChanged(const QString &arg1);

    void updateInformation();

    void updateInformation(const QString &arg1);

    void on_vectorInputRadioBtn_clicked();

    void on_matrixInputRadioBtn_clicked();

    void on_pushButton_clicked();

    void on_helpAction_triggered();

private:
    Ui::MainWindow *ui;

    matrixCalc *matrixCalcEntity;

    bool isValidInput;

    QVector<unsigned int> inputSequence;
//    QVector<QVector<unsigned int>>* outputData;


    void updateLineEditList();



    bool isDigitalString(QString str);

    QVector<unsigned int> getVectorIntFromStr(QString str);
    QString getStrFromVectorInt(QVector<unsigned int>);

    QString inputValidation(int state);

    bool firstValidation(QString* errorStr);
    bool secondValidation(QString* errorStr);
    bool thirdValidation(QString* errorStr);
    bool fouthValidation(QString* errorStr);
    bool fifthValidation(QString* errorStr);

    bool validateFormulaByRegEx(int dimension);
    QString validateAllLineEdit();

    unsigned short simpleValidate(QString inputStr);
    QString errorValidateDecode(unsigned short error);

    void saveLogFile(QString prefix, QString data);

    QVector<QLineEdit*> lineEditList;



};
#endif // MAINWINDOW_H
