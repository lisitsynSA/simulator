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
    m_display = new Display(this);
    ui->gridLayoutWidget->layout()->addWidget(m_display);
    m_life = new Life(1000, 600, this);
    connect(ui->actionStart_life, SIGNAL(triggered(bool)), m_life, SLOT(startGame()));
    connect(ui->actionAdd_virus, SIGNAL(triggered(bool)), m_life, SLOT(addVirus()));
    connect(m_life, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));
    uint32_t array[100];
    m_display->loadARGB32(array, 10, 10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

