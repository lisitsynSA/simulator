#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "display.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->gridLayoutWidget);
    m_display = new Display(ui->graphicsView, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

