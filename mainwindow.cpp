#include "mainwindow.h"
#include "DEMO_interface.h"
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
#include <QTimer>
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

  // DEMO
  m_demoThread = new DemoThread();
  connect(ui->actionDEMO, SIGNAL(triggered(bool)), this, SLOT(startDEMO()));
  connect(ui->actionDEMO_LLVM, SIGNAL(triggered(bool)), this,
          SLOT(startDEMO_LLVM()));
  m_updTimer = new QTimer(this);
  connect(m_updTimer, SIGNAL(timeout()), this, SLOT(updDisplay()));

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
         " Registers: r0-r15"
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
         " <h3>ASM Prologue:</h3>"
         " MOVli [stack reg] 32767<br>"
         " BL [link reg] main<br>"
         " EXIT<br><br>"
         " simRand:<br>"
         " RAND [arg0 reg]<br>"
         " BR [link reg]<br><br>"
         " simFlush:<br>"
         " FLUSH<br>"
         " BR [link reg]<br><br>"
         " simBkpt:<br>"
         " BKPT<br>"
         " BR [link reg]<br><br>"
         " simSetPixel:<br>"
         " MULi [tmp reg] [arg1 reg] 256<br>"
         " ADDi [tmp reg] [tmp reg] 32767<br>"
         " ADDi [tmp reg] [tmp reg] 1<br>"
         " ST [arg2 reg] [tmp reg] [arg0 reg]<br>"
         " BR [link reg]<br>"
#define _ISA(_opcode, _name, _execute, _asmargs, _disasmargs, _dumpregs)       \
  "<p><h3> " #_opcode " " #_name "</h3><p>" #_execute
#include "ISA.h"
#undef _ISA
         " <h3>Example 'return':</h3>"
         " MOVli r2 32767<br>"
         " BL r1 main<br>"
         " EXIT<br><br>"
         "simRand:<br>"
         " RAND r9<br>"
         " BR r1<br><br>"
         "simFlush:<br>"
         " FLUSH<br>"
         " BR r1<br><br>"
         "simBkpt:<br>"
         " BKPT<br>"
         " BR r1<br><br>"
         "simSetPixel:<br>"
         " MULi r13 r10 256<br>"
         " ADDi r13 r13 32767<br>"
         " ADDi r13 r13 1<br>"
         " ST r11 r13 r9<br>"
         " BR r1<br><br>"
         "main:<br>"
         " MOVli r9 0<br>"
         " BR r1<br>"

         "<h3>Example 'simSetPixel':</h3>"
         " MOVli r2 32767<br>"
         " BL r1 main<br>"
         " EXIT<br><br>"
         "simRand:<br>"
         " RAND r9<br>"
         " BR r1<br><br>"
         "simFlush:<br>"
         " FLUSH<br>"
         " BR r1<br><br>"
         "simBkpt:<br>"
         " BKPT<br>"
         " BR r1<br><br>"
         "simSetPixel:<br>"
         " MULi r13 r10 256<br>"
         " ADDi r13 r13 32767<br>"
         " ADDi r13 r13 1<br>"
         " ST r11 r13 r9<br>"
         " BR r1<br><br>"
         "main:<br>"
         " ADDi r2 r2 -1<br>"
         " STi r1 r2 0<br>"
         " MOVli r9 0<br>"
         " MOVli r10 127<br>"
         " MOVhi r11 65535<br>"
         " BL r1 simSetPixel<br>"
         " BL r1 simFlush<br>"
         " LDi r1 r2 0<br>"
         " ADDi r2 r2 1<br>"
         " BR r1<br>"

         "<h3>Example 'Random':</h3>"
         " MOVli r2 32767<br>"
         " BL r1 main<br>"
         " EXIT<br><br>"
         "simRand:<br>"
         " RAND r9<br>"
         " BR r1<br><br>"
         "simFlush:<br>"
         " FLUSH<br>"
         " BR r1<br><br>"
         "simBkpt:<br>"
         " BKPT<br>"
         " BR r1<br><br>"
         "simSetPixel:<br>"
         " MULi r13 r10 256<br>"
         " ADDi r13 r13 32767<br>"
         " ADDi r13 r13 1<br>"
         " ST r11 r13 r9<br>"
         " BR r1<br><br>"
         "main:<br>"
         " ADDi r2 r2 -7<br>"
         " STi r1 r2 6<br>"
         " STi r3 r2 5<br>"
         " ADDi r3 r2 7<br>"
         " MOVli r4 0<br>"
         " STi r4 r3 -3<br>"
         " STi r4 r3 -4<br>"
         " STi r4 r3 -5<br>"
         ".LBB0_1:<br>"
         " LDi r4 r3 -5<br>"
         " MOVli r9 127<br>"
         " B.GT r4 r9 .LBB0_8<br>"
         " MOVli r4 0<br>"
         " STi r4 r3 -4<br>"
         ".LBB0_3:<br>"
         " LDi r4 r3 -4<br>"
         " MOVli r9 255<br>"
         " B.GT r4 r9 .LBB0_6<br>"
         " LDi r4 r3 -4<br>"
         " STi r4 r3 -6<br>"
         " LDi r4 r3 -5<br>"
         " STi r4 r3 -7<br>"
         " BL r1 simRand<br>"
         " LDi r10 r3 -7<br>"
         " ADDi r11 r9 0<br>"
         " LDi r9 r3 -6<br>"
         " BL r1 simSetPixel<br>"
         " LDi r4 r3 -4<br>"
         " ADDi r4 r4 1<br>"
         " STi r4 r3 -4<br>"
         " B .LBB0_3<br>"
         ".LBB0_6:<br>"
         " BL r1 simFlush<br>"
         " LDi r4 r3 -5<br>"
         " ADDi r4 r4 1<br>"
         " STi r4 r3 -5<br>"
         " B .LBB0_1<br>"
         ".LBB0_8:<br>"
         " MOVli r9 0<br>"
         " LDi r3 r2 5<br>"
         " LDi r1 r2 6<br>"
         " ADDi r2 r2 7<br>"
         " BR r1<br>"

         "<h3>Example 'RED':</h3>"
         " MOVli r2 32767<br>"
         " BL r1 main<br>"
         " EXIT<br><br>"
         "simRand:<br>"
         " RAND r9<br>"
         " BR r1<br><br>"
         "simFlush:<br>"
         " FLUSH<br>"
         " BR r1<br><br>"
         "simBkpt:<br>"
         " BKPT<br>"
         " BR r1<br><br>"
         "simSetPixel:<br>"
         " MULi r13 r10 256<br>"
         " ADDi r13 r13 32767<br>"
         " ADDi r13 r13 1<br>"
         " ST r11 r13 r9<br>"
         " BR r1<br><br>"
         " main:<br>"
         " ADDi r2 r2 -6<br>"
         " STi r1 r2 5<br>"
         " STi r3 r2 4<br>"
         " ADDi r3 r2 6<br>"
         " MOVli r4 0<br>"
         " STi r4 r3 -3<br>"
         " STi r4 r3 -4<br>"
         " STi r4 r3 -5<br>"
         " STi r4 r3 -6<br><br>"
         ".loopRepeat:<br>"
         " LDi r4 r3 -6<br>"
         " MOVli r9 254<br>"
         " B.GT r4 r9 .exit<br>"
         " MOVli r4 0<br>"
         " STi r4 r3 -5<br><br>"
         ".loopY:<br>"
         " LDi r4 r3 -5<br>"
         " MOVli r9 127<br>"
         " B.GT r4 r9 .exitY<br>"
         " MOVli r4 0<br>"
         " STi r4 r3 -4<br><br>"
         ".loopX:<br>"
         " LDi r4 r3 -4<br>"
         " MOVli r9 255<br>"
         " B.GT r4 r9 .exitX<br>"
         " LDi r9 r3 -4<br>"
         " LDi r10 r3 -5<br>"
         " LDi r4 r3 -6<br>"
         " MULi r4 r4 257<br>"
         " MOVhi r11 65535<br>"
         " ADD r11 r4 r11<br>"
         " BL r1 simSetPixel<br>"
         " LDi r4 r3 -4<br>"
         " ADDi r4 r4 1<br>"
         " STi r4 r3 -4<br>"
         " B .loopX<br><br>"
         ".exitX:<br>"
         " LDi r4 r3 -5<br>"
         " ADDi r4 r4 1<br>"
         " STi r4 r3 -5<br>"
         " B .loopY<br><br>"
         ".exitY:<br>"
         " BL r1 simFlush<br>"
         " LDi r4 r3 -6<br>"
         " ADDi r4 r4 1<br>"
         " STi r4 r3 -6<br>"
         " B .loopRepeat<br><br>"
         ".exit:<br>"
         " MOVli r9 0<br>"
         " LDi r3 r2 4<br>"
         " LDi r1 r2 5<br>"
         " ADDi r2 r2 6<br>"
         " BR r1<br><br>"
         "<p>Lisitsyn Sergey (s.a.lisitsyn@gmail.com) 2022"));
}

// DEMO
void MainWindow::startDEMO() {
  m_updTimer->start(10);
  m_demoThread->isLLVM = false;
  m_demoThread->start();
}

void MainWindow::startDEMO_LLVM() {
  m_updTimer->start(10);
  m_demoThread->isLLVM = true;
  m_demoThread->start();
}

extern uint32_t *DEMO_data;
void MainWindow::updDisplay() {
  if (DEMO_data) {
    m_display->loadARGB32Scaled(DEMO_data, DEMO_SIZE_X, DEMO_SIZE_Y, DIS_SCALE);
    DEMO_data = nullptr;
  }
}

extern void DEMO_main();
extern void DEMO_LLVM_main();
void DemoThread::run() {
  if (isLLVM) {
    DEMO_LLVM_main();
  } else {
    DEMO_main();
  }
}

void MainWindow::showMsg(const QString &msg) { m_statusLabel.setText(msg); }

void MainWindow::loadCode() { m_cpu->readInstrs(ui->codeEdit->toPlainText()); }

MainWindow::~MainWindow() {
  delete ui;
  delete m_demoThread;
}
