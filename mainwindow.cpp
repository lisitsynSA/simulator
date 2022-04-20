#include "mainwindow.h"
#include "cpu.h"
#include "display.h"
#include "life.h"
#include "mapgenerator.h"
#include "relaxation.h"
#include "relaxationtor.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  m_statusLabel.setAlignment(Qt::AlignHCenter);
  statusBar()->addWidget(&m_statusLabel);
  setWindowTitle("CPU Emulator");
  setCentralWidget(ui->gridLayoutWidget);
  m_display = new Display(this);
  m_display->setMinimumSize((DIS_WIDTH + 1) * DIS_SCALE,
                            (DIS_HEIGHT + 1) * DIS_SCALE);
  m_display->setMaximumSize((DIS_WIDTH + 1) * DIS_SCALE,
                            (DIS_HEIGHT + 1) * DIS_SCALE);
  ui->displayLayout->layout()->addWidget(m_display);

  // Game of Life
  m_life = new Life(DIS_WIDTH * DIS_SCALE, DIS_HEIGHT * DIS_SCALE, this);
  connect(ui->actionLife, SIGNAL(triggered(bool)), m_life, SLOT(startGame()));
  connect(ui->actionRelaxation, SIGNAL(triggered(bool)), m_life,
          SLOT(stopGame()));
  connect(ui->actionTorRelaxation, SIGNAL(triggered(bool)), m_life,
          SLOT(stopGame()));
  connect(ui->actionStopDemo, SIGNAL(triggered(bool)), m_life,
          SLOT(stopGame()));
  connect(ui->actionStep, SIGNAL(triggered(bool)), m_life, SLOT(stopGame()));
  connect(ui->actionRun_CPU, SIGNAL(triggered(bool)), m_life, SLOT(stopGame()));
  connect(ui->actionLocusGen, SIGNAL(triggered(bool)), m_life,
          SLOT(stopGame()));
  connect(ui->actionHeightGen, SIGNAL(triggered(bool)), m_life,
          SLOT(stopGame()));
  connect(ui->actionRiverGen, SIGNAL(triggered(bool)), m_life,
          SLOT(stopGame()));
  connect(m_life, SIGNAL(sendSpace(uint32_t *, uint32_t, uint32_t)), m_display,
          SLOT(loadARGB32(uint32_t *, uint32_t, uint32_t)));
  connect(m_display, SIGNAL(leftClick(uint32_t, uint32_t)), m_life,
          SLOT(addLife(uint32_t, uint32_t)));
  connect(m_display, SIGNAL(rightClick(uint32_t, uint32_t)), m_life,
          SLOT(addVirus(uint32_t, uint32_t)));

  // Relaxation
  m_relax = new Relaxation(DIS_WIDTH * DIS_SCALE, DIS_HEIGHT * DIS_SCALE, this);
  connect(ui->actionRelaxation, SIGNAL(triggered(bool)), m_relax,
          SLOT(startRelaxation()));
  connect(ui->actionLife, SIGNAL(triggered(bool)), m_relax,
          SLOT(stopRelaxation()));
  connect(ui->actionTorRelaxation, SIGNAL(triggered(bool)), m_relax,
          SLOT(stopRelaxation()));
  connect(ui->actionStopDemo, SIGNAL(triggered(bool)), m_relax,
          SLOT(stopRelaxation()));
  connect(ui->actionStep, SIGNAL(triggered(bool)), m_relax,
          SLOT(stopRelaxation()));
  connect(ui->actionRun_CPU, SIGNAL(triggered(bool)), m_relax,
          SLOT(stopRelaxation()));
  connect(ui->actionLocusGen, SIGNAL(triggered(bool)), m_relax,
          SLOT(stopRelaxation()));
  connect(ui->actionHeightGen, SIGNAL(triggered(bool)), m_relax,
          SLOT(stopRelaxation()));
  connect(ui->actionRiverGen, SIGNAL(triggered(bool)), m_relax,
          SLOT(stopRelaxation()));
  connect(m_relax, SIGNAL(sendSpace(uint32_t *, uint32_t, uint32_t)), m_display,
          SLOT(loadARGB32(uint32_t *, uint32_t, uint32_t)));

  // Tor Relaxation
  m_relaxTor =
      new RelaxationTor(DIS_WIDTH * DIS_SCALE, DIS_HEIGHT * DIS_SCALE, this);
  connect(ui->actionTorRelaxation, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(startRelaxation()));
  connect(ui->actionLife, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(stopRelaxation()));
  connect(ui->actionRelaxation, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(stopRelaxation()));
  connect(ui->actionStopDemo, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(stopRelaxation()));
  connect(ui->actionStep, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(stopRelaxation()));
  connect(ui->actionRun_CPU, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(stopRelaxation()));
  connect(ui->actionLocusGen, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(stopRelaxation()));
  connect(ui->actionHeightGen, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(stopRelaxation()));
  connect(ui->actionRiverGen, SIGNAL(triggered(bool)), m_relaxTor,
          SLOT(stopRelaxation()));
  connect(m_relaxTor, SIGNAL(sendSpace(uint32_t *, uint32_t, uint32_t)),
          m_display, SLOT(loadARGB32(uint32_t *, uint32_t, uint32_t)));
  // Map Generation
  m_mapGen =
      new MapGenerator(DIS_WIDTH * DIS_SCALE, DIS_HEIGHT * DIS_SCALE, this);
  connect(ui->actionLocusGen, SIGNAL(triggered(bool)), m_mapGen,
          SLOT(startRelaxation()));
  connect(ui->actionHeightGen, SIGNAL(triggered(bool)), m_mapGen,
          SLOT(generateHeight()));
  connect(ui->actionRiverGen, SIGNAL(triggered(bool)), m_mapGen,
          SLOT(riverGeneration()));
  connect(m_mapGen, SIGNAL(sendSpace(uint32_t *, uint32_t, uint32_t)),
          m_display, SLOT(loadARGB32(uint32_t *, uint32_t, uint32_t)));
  connect(m_display, SIGNAL(leftClick(uint32_t, uint32_t)), m_mapGen,
          SLOT(select(uint32_t, uint32_t)));

  // CPU
  m_cpu = new CPU(this);
  connect(ui->actionLoad_code, SIGNAL(triggered(bool)), this, SLOT(loadCode()));
  connect(ui->actionRun_CPU, SIGNAL(triggered(bool)), m_cpu, SLOT(run()));
  connect(ui->actionPause_CPU, SIGNAL(triggered(bool)), m_cpu, SLOT(pause()));
  connect(ui->actionStop_CPU, SIGNAL(triggered(bool)), m_cpu, SLOT(stop()));
  connect(ui->actionStep, SIGNAL(triggered(bool)), m_cpu, SLOT(step()));
  connect(m_cpu, SIGNAL(statusUpd(QString)), ui->cpuEdit,
          SLOT(setPlainText(QString)));
  connect(m_cpu, SIGNAL(memUpd(QString)), ui->hexEdit,
          SLOT(setPlainText(QString)));
  connect(m_cpu, SIGNAL(displayUpd(uint32_t *, uint32_t, uint32_t, uint32_t)),
          m_display,
          SLOT(loadARGB32Scaled(uint32_t *, uint32_t, uint32_t, uint32_t)));
  connect(m_cpu, SIGNAL(sendMsg(QString)), this, SLOT(showMsg(QString)));
  m_cpu->dumpStatus();
  m_cpu->dumpMem();
  m_cpu->showMsg("CPU is ready");

  QToolBar *toolBar = addToolBar("&Edit");
  toolBar->addAction(ui->actionLoad_code);
  toolBar->addAction(ui->actionRun_CPU);
  toolBar->addAction(ui->actionPause_CPU);
  toolBar->addAction(ui->actionStop_CPU);
  toolBar->addAction(ui->actionStep);

  ui->ISABrowser->setText(
      tr("<h2>CPU Emulator ISA v0.1</h2>"
#define _ISA(_opcode, _name, _execute, _asmargs, _disasmargs, _dumpregs)       \
  "<p><h3> " #_opcode " " #_name "</h3><p>" #_execute " "
#include "ISA.h"
#undef _ISA
         "<p>Lisitsyn Sergey (s.a.lisitsyn@gmail.com) 2021"));
}

void MainWindow::showMsg(const QString &msg) { m_statusLabel.setText(msg); }

void MainWindow::loadCode() { m_cpu->readInstrs(ui->codeEdit->toPlainText()); }

MainWindow::~MainWindow() { delete ui; }
