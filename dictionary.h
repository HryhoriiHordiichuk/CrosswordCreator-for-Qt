#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class Dictionary;
}

class Dictionary : public QDialog
{
    Q_OBJECT

public:
    explicit Dictionary(QWidget *parent = nullptr);
    ~Dictionary();
signals:
    void wordCount(int count);
private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dictionary *ui;
};

#endif // DICTIONARY_H
