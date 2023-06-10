#include "main_start.h"
#include "ui_main_start.h"

main_start::main_start(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::main_start)
{
    ui->setupUi(this);
}

main_start::~main_start()
{
    delete ui;
}
