#include "dialogvved.h"
#include "ui_dialogvved.h"
#include <QMessageBox>
#include <QRegExpValidator>
#include <QRegExp>

DialogVved::DialogVved(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogVved)
{
    ui->setupUi(this);
    setLayout(ui->horizontalLayout_4);
    QRegExpValidator *validator = new QRegExpValidator(this);
    validator->setRegExp(QRegExp("[А-ЩЮЯҐЄІЇ]*[а-щьюяґєії'’ʼ]*[A-z]*"));
    ui->lineEdit->setValidator(validator);
}

DialogVved::~DialogVved()
{
    delete ui;
}
void DialogVved::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty())
    {
        if(ui->checkBox->isChecked()){
            emit word(ui->lineEdit->text().toLower(),ui->lineEdit_2->text().toLower(),true);
        }
        else{
            emit word(ui->lineEdit->text().toLower(),ui->lineEdit_2->text().toLower(),false);
        }
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
    else{
        QMessageBox::information(this,"Error","Заповніть усі поля!");
    }
}
