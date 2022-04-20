#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  qRegisterMetaType<uint32_t>("uint32_t");
  MainWindow w;
  w.show();
  return a.exec();
}
