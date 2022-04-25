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
         "<h3>Instr format:</h3>"
         "  FF FF FF FF<br>"
         "  __ __ ^^^^ - r3_imm - third reg or imm<br>"
         "  __ _^ __ __ - r2     - second reg<br>"
         "  __ ^_ __ __ - r1     - first reg<br>"
         "  ^^__ __ __ - opcode - operation code<p>"
         " <h3>Registers:</h3> r0-r15"
         " <h3>Memory:</h3>"
         " MEM_SIZE = 65536 words (32 bits);<br>"
         " 0x0000 ↓ CODE + DATA<br>"
         " ...<br>"
         " 0x8000 ↑ STACK<br>"
         " ...    DISPLAY MEM (32768 words == 256*128 ARGB)<br>"
         " 0xFFFF"
         " <h3>Header:</h3>"
         " #define SIM_X_SIZE 256<br>"
         " #define SIM_Y_SIZE 128<p>"
         " extern void simSetPixel(int x, int y, int rgb);<br>"
         " extern void simFlush();<br>"
         " extern void simBkpt();<br>"
         " extern int simRand();"
         " <h3>Example:</h3>"
         " BL r1 test_app<br>"
         " EXIT<br><br>"
         " test_app:<br>"
         " MOVli r1 32768<br>"
         " SUBi r2 r1 1<br>"
         " MOVhi r3 61440<br>"
         " XOR r4 r4 r4<br><br>"
         " loop:<br>"
         " B.EQ r2 r4 return<br>"
         " ST r3 r1 r2<br>"
         " SUBi r2 r2 1<br>"
         " ADDi r3 r3 4112<br>"
         " FLUSH<br>"
         " ANDi r5 r2 255<br>"
         " B.EQ r5 r4 break<br>"
         " B loop<br><br>"
         " break:<br>"
         " BKPT<br>"
         " B loop<br><br>"
         " return:<br>"
         " BR r1"
#define _ISA(_opcode, _name, _execute, _asmargs, _disasmargs, _dumpregs)       \
  "<p><h3> " #_opcode " " #_name "</h3><p><pre>" #_execute "</pre> "
#include "ISA.h"
#undef _ISA
         "<p>Lisitsyn Sergey (s.a.lisitsyn@gmail.com) 2022"));
}

void MainWindow::showMsg(const QString &msg) { m_statusLabel.setText(msg); }

void MainWindow::loadCode() { m_cpu->readInstrs(ui->codeEdit->toPlainText()); }

MainWindow::~MainWindow() { delete ui; }
