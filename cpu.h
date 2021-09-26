#ifndef CPU_H
#define CPU_H

#include <QObject>
#include "instr.h"

const uint32_t REG_SIZE = 16;
const uint32_t MEM_SIZE = 65536;
const uint32_t DIS_WIDTH = 256;
const uint32_t DIS_HEIGHT = 128;

class CPU : public QObject
{
    Q_OBJECT
public:
    explicit CPU(QObject *parent = nullptr);
public slots:
    void readInstrs(QString input);
    void run();
    void dumpStatus();
    void dumpMem();
    void updateDisplay();
signals:
    void statusUpd(QString status);
    void memUpd(QString status);
    void displayUpd(uint32_t *mem, uint32_t width, uint32_t height);
public:
    uint32_t m_regFile[REG_SIZE] = {};
    uint32_t m_mem[MEM_SIZE] = {};
    uint32_t m_PC;
    uint32_t m_nextPC;
    uint32_t m_run;
};

#endif // CPU_H
