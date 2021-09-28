#include "mainwindow.h"

#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType < uint32_t >("uint32_t");
    MainWindow w;
    w.show();
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    return a.exec();
}
