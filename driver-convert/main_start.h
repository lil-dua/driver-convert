#ifndef MAIN_START_H
#define MAIN_START_H

#include <QDialog>

namespace Ui {
class main_start;
}

class main_start : public QDialog
{
    Q_OBJECT

public:
    explicit main_start(QWidget *parent = 0);
    ~main_start();

private:
    Ui::main_start *ui;
};

#endif // MAIN_START_H
