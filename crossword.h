#ifndef Crossword_H
#define Crossword_H

#include <QMainWindow>
#include <QItemDelegate>
#include <QPainter>
#include <QTableWidgetItem>
#include <QWebEngineView>

namespace Ui {
class Crossword;
}
class MyDelegate : public QItemDelegate {
  public:
    MyDelegate( QObject *parent ) : QItemDelegate( parent ) { }
    void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
class Crossword : public QMainWindow
{
    Q_OBJECT

public:
    explicit Crossword(QWidget *parent = nullptr);
    ~Crossword();
    QStringList strList;
    QStringList strList2;
    QStringList writeStrText;
    QStringList writeStrText2;
    QStringList writeStr;
    QStringList questionList;
    QWebEngineView *page;

private slots:
    void on_pushButton_clicked();
    void addWord(QString str,QString str1, bool cheked);
    void on_pushButton_5_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void setWordCount(int count);
    void fromPdf(bool a);
    void cellDelegate(int row,int column);

    void on_pushButton_3_clicked();

signals:
private:
    Ui::Crossword *ui;
};

#endif // Crossword_H
