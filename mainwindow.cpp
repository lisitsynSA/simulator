#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "display.h"
#include "life.h"
#include "relaxation.h"
#include "relaxationtor.h"
#include "mapgenerator.h"
#include "cpu.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(QDateTime::currentDateTimeUtc().toTime_t());
    setCentralWidget(ui->gridLayoutWidget);
    m_display = new Display(this);
    m_display->setMinimumSize((DIS_WIDTH + 1) * DIS_SCALE,\
                              (DIS_HEIGHT + 1) * DIS_SCALE);
    m_display->setMaximumSize((DIS_WIDTH + 1) * DIS_SCALE,\
                              (DIS_HEIGHT + 1) * DIS_SCALE);
    ui->displayLayout->layout()->addWidget(m_display);

    // Game of Life
    m_life = new Life(800, 800, this);
    connect(ui->actionStart_life, SIGNAL(triggered(bool)), m_life, SLOT(startGame()));
    connect(ui->actionStart_relax, SIGNAL(triggered(bool)), m_life, SLOT(stopGame()));
    connect(ui->actionStart_relax_tor, SIGNAL(triggered(bool)), m_life, SLOT(stopGame()));
    connect(m_life, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));
    connect(m_display, SIGNAL(leftClick(uint32_t,uint32_t)), m_life, SLOT(addLife(uint32_t,uint32_t)));
    connect(m_display, SIGNAL(rightClick(uint32_t,uint32_t)), m_life, SLOT(addVirus(uint32_t,uint32_t)));

    // Relaxation
    m_relax = new Relaxation(600, 600, this);
    connect(ui->actionStart_relax, SIGNAL(triggered(bool)), m_relax, SLOT(startRelaxation()));
    connect(ui->actionStart_life, SIGNAL(triggered(bool)), m_relax, SLOT(stopRelaxation()));
    connect(ui->actionStart_relax_tor, SIGNAL(triggered(bool)), m_relax, SLOT(stopRelaxation()));
    connect(m_relax, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));

    // Tor Relaxation
    m_relaxTor = new RelaxationTor(600, 600, this);
    connect(ui->actionStart_relax_tor, SIGNAL(triggered(bool)), m_relaxTor, SLOT(startRelaxation()));
    connect(ui->actionStart_life, SIGNAL(triggered(bool)), m_relaxTor, SLOT(stopRelaxation()));
    connect(ui->actionStart_relax, SIGNAL(triggered(bool)), m_relaxTor, SLOT(stopRelaxation()));
    connect(m_relaxTor, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));
    // Map Generation
    m_mapGen = new MapGenerator(1200, 600, this);
    connect(ui->actionLocusGen, SIGNAL(triggered(bool)), m_mapGen, SLOT(startRelaxation()));
    connect(ui->actionHeightGen, SIGNAL(triggered(bool)), m_mapGen, SLOT(generateHeight()));
    connect(ui->actionRiverGen, SIGNAL(triggered(bool)), m_mapGen, SLOT(riverGeneration()));
    connect(m_mapGen, SIGNAL(sendSpace(uint32_t*,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32(uint32_t*,uint32_t,uint32_t)));
    connect(m_display, SIGNAL(leftClick(uint32_t,uint32_t)), m_mapGen, SLOT(select(uint32_t,uint32_t)));

    // CPU
    m_cpu = new CPU(this);
    connect(ui->actionLoad_code, SIGNAL(triggered(bool)), this, SLOT(loadCode()));
    connect(ui->actionRun_CPU, SIGNAL(triggered(bool)), m_cpu, SLOT(run()));
    connect(ui->actionPause_CPU, SIGNAL(triggered(bool)), m_cpu, SLOT(pause()));
    connect(ui->actionStop_CPU, SIGNAL(triggered(bool)), m_cpu, SLOT(stop()));
    connect(ui->actionStep, SIGNAL(triggered(bool)), m_cpu, SLOT(step()));
    connect(m_cpu, SIGNAL(statusUpd(QString)), ui->cpuEdit, SLOT(setPlainText(QString)));
    connect(m_cpu, SIGNAL(memUpd(QString)), ui->hexEdit, SLOT(setPlainText(QString)));
    connect(m_cpu, SIGNAL(displayUpd(uint32_t*,uint32_t,uint32_t,uint32_t)),
            m_display, SLOT(loadARGB32Scaled(uint32_t*,uint32_t,uint32_t,uint32_t)));
    m_cpu->dumpStatus();
    m_cpu->dumpMem();
}

void MainWindow::loadCode() {
    m_cpu->readInstrs(ui->codeEdit->toPlainText());
}

MainWindow::~MainWindow()
{
    delete ui;
}

