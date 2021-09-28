#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QThread>
#include "instr.h"

const uint32_t REG_SIZE = 16;
const uint32_t MEM_SIZE = 65536;
const uint32_t DIS_WIDTH = 256;
const uint32_t DIS_HEIGHT = 128;
const uint32_t DIS_SCALE = 4;

class CPU;

class CPUExecThreads : public QThread
{
public:
    explicit CPUExecThreads(CPU *cpu_init) {
        cpu = cpu_init;
    };
    CPU *cpu;
    void run();
};

class CPU : public QObject
{
    Q_OBJECT
public:
    explicit CPU(QObject *parent = nullptr);
    ~CPU();
    std::string processLabels(std::string input);
public slots:
    void readInstrs(QString input);
    void run();
    void pause();
    void stop();
    void dumpStatus();
    void dumpMem();
    void updateDisplay();
signals:
    void statusUpd(QString status);
    void memUpd(QString status);
    void displayUpd(uint32_t *mem, uint32_t width, uint32_t height, uint32_t scale);
public:
    int32_t m_regFile[REG_SIZE] = {};
    uint32_t m_mem[MEM_SIZE] = {};
    uint32_t m_PC;
    uint32_t m_nextPC;
    uint32_t m_run;
    CPUExecThreads *execThread;
};

#endif // CPU_H
