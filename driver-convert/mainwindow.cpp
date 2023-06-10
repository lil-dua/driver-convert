#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main_start.h"
#include "qmessagebox.h"

static QString tk;
static QString mk;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_dangnhap_clicked()
{
    QMessageBox* msgbox = new QMessageBox(this);
    msgbox->setWindowTitle("Thông báo");

    tk = ui->taikhoan->text();
    mk = ui->matkhau->text();
    if(tk == "admin" && mk == "admin")
    {

        hide();
        main_start a ;
        a.setModal(true);
        a.exec();
    }else
    {
        msgbox->setText("Sai tài khoản hoặc mật khẩu");
        msgbox->open();
    }
}
