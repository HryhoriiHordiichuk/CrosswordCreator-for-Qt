#ifndef DIALOGVVED_H
#define DIALOGVVED_H

#include <QDialog>

namespace Ui {
class DialogVved;
}

class DialogVved : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVved(QWidget *parent = nullptr);
    ~DialogVved();
signals:
   void word(QString str,QString str1,bool cheked);
private slots:
   void on_pushButton_clicked();
private:
    Ui::DialogVved *ui;
};

#endif // DIALOGVVED_H
