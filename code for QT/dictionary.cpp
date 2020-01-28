#include "dictionary.h"
#include "ui_dictionary.h"
#include "crossword.h"
#include <QDialogButtonBox>
#include <QRegExpValidator>
#include <QFile>
#include <QDir>
#include <QMessageBox>

Dictionary::Dictionary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dictionary)
{
    ui->setupUi(this);
    setLayout(ui->horizontalLayout_3);
    QRegExpValidator *validator = new QRegExpValidator(this);
    validator->setRegExp(QRegExp("[0-9]*"));
    ui->lineEdit->setValidator(validator);
}

Dictionary::~Dictionary()
{
    delete ui;
}



void Dictionary::on_pushButton_clicked()
{
    this->close();
}

void Dictionary::on_pushButton_2_clicked()
{
    QFile *file = new QFile;
    file->setFileName(QApplication::applicationDirPath() + "/list.txt");
    if(!file->open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qCritical("Error %d: %s",file->error(),qPrintable(file->errorString()));
    }
    else {
        int count=0;
        QString str;
        do{
            str=file->readLine();
            if(!str.isNull()){
                count++;
            }
        }while(!str.isNull());
        file->close();
        if(!ui->lineEdit->text().isEmpty()){
            if(ui->lineEdit->text().toInt()>count){
                QMessageBox::information(this,"Error","Кількість слів більша ніж є у словнику, введіть меншу кількість");
            }
            else {
                emit wordCount(ui->lineEdit->text().toInt());
                this->close();
            }
        }
    }
}
