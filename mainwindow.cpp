#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "display.h"
#include "memory.h"
#include "life.h"
#include "relaxation.h"
#include "relaxationtor.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(QDateTime::currentDateTime().toTime_t());
    setCentralWidget(ui->gridLayoutWidget);
    m_display = new Display(this);
    ui->gridLayoutWidget->layout()->addWidget(m_display);
    //m_mem = new Memory(this);
    //ui->gridLayoutWidget->layout()->addWidget(m_mem);

    // Game of Life
    m_life = new Life(1000, 600, this);
    connect(ui->actionStart_life, SIGNAL(triggered(bool)), m_life, SLOT(startGame()));
    connect(m_life, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));
    connect(m_display, SIGNAL(leftClick(uint32_t,uint32_t)), m_life, SLOT(addLife(uint32_t,uint32_t)));
    connect(m_display, SIGNAL(rightClick(uint32_t,uint32_t)), m_life, SLOT(addVirus(uint32_t,uint32_t)));

    // Relaxation
    m_relax = new Relaxation(200, 200, this);
    connect(ui->actionStart_relax, SIGNAL(triggered(bool)), m_relax, SLOT(startRelaxation()));
    connect(m_relax, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));

    // Tor Relaxation
    m_relaxTor = new RelaxationTor(200, 200, this);
    connect(ui->actionStart_relax_tor, SIGNAL(triggered(bool)), m_relaxTor, SLOT(startRelaxation()));
    connect(m_relaxTor, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

