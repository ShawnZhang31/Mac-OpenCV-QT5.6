#include "mainwindow.h"
#include "ui_mainwindow.h"

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

/**
 * @brief MainWindow::on_actionOpenTestImage_triggered
 * 打开测试文件
 */
void MainWindow::on_actionOpenTestImage_triggered()
{

}

/**
 * @brief MainWindow::on_actionExit_triggered
 * 退出程序
 */
void MainWindow::on_actionExit_triggered()
{

}
