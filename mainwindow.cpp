#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "display.h"
#include "life.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->gridLayoutWidget);
    m_display = new Display(ui->graphicsView, this);
    m_life = new Life(1000, 600, this);
    connect(ui->actionDebug_signal, SIGNAL(triggered(bool)), m_life, SLOT(startGame()));
    connect(m_life, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

