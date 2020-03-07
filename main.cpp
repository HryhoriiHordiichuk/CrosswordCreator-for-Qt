#include "crossword.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Crossword w;
    w.setWindowFlags(w.windowFlags() | Qt::Dialog);
    w.show();
    return a.exec();
}
