#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QThread>
class QTimer;
class Display;
class Memory;
class Life;
class Relaxation;
class RelaxationTor;
class MapGenerator;
class CPU;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class DemoThread : public QThread {
public:
  explicit DemoThread(){};
  void run();
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
public slots:
  void loadCode();
  void showMsg(const QString &msg);
  void startDEMO();
  void updDisplay();

private:
  Ui::MainWindow *ui;
  Display *m_display;
  Memory *m_mem;
  Life *m_life;
  Relaxation *m_relax;
  RelaxationTor *m_relaxTor;
  MapGenerator *m_mapGen;
  CPU *m_cpu;
  QLabel m_statusLabel;
  DemoThread *m_demoThread;

  QTimer *m_updTimer;
};
#endif // MAINWINDOW_H
