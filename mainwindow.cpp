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
    connect(ui->actionRegenerate, SIGNAL(triggered(bool)), m_life, SLOT(generateSpace()));
    connect(m_life, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));
    connect(m_display, SIGNAL(leftClick(uint32_t,uint32_t)), m_life, SLOT(addLife(uint32_t,uint32_t)));
    connect(m_display, SIGNAL(rightClick(uint32_t,uint32_t)), m_life, SLOT(addVirus(uint32_t,uint32_t)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

