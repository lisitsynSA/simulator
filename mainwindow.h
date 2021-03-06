#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
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

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
public slots:
  void loadCode();
  void showMsg(const QString &msg);

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
};
#endif // MAINWINDOW_H
