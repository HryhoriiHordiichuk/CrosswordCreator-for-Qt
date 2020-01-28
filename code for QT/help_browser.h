#ifndef HELP_BROWSER_H
#define HELP_BROWSER_H

#include <QDialog>

namespace Ui {
class help_browser;
}

class help_browser : public QDialog
{
    Q_OBJECT

public:
    explicit help_browser(QWidget *parent = nullptr);
    ~help_browser();

private:
    Ui::help_browser *ui;
};

#endif // HELP_BROWSER_H
