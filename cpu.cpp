#include "cpu.h"
#include <sstream>
#include <QDebug>
#include <iomanip>

CPU::CPU(QObject *parent) : QObject(parent)
{
    Instr::prepareDict();
}

void CPU::readInstrs(QString input_string){
    std::stringstream input;
    input.str(input_string.toStdString());
    Instr instr;
    for (uint32_t mem = 0; mem < MEM_SIZE; mem++) {
        if (input.rdbuf()->in_avail()) {
            instr.assembler(input);
            m_mem[mem] = instr.code();
        } else {
            m_mem[mem] = 0;
        }
    }
    dumpStatus();
    dumpMem();
}

void CPU::run() {
    m_PC = 0;
    m_run = true;
    Instr instr;
    while (m_run) {
        m_nextPC = m_PC + 1;
        instr.executeCode(this, m_mem[m_PC]);
        m_PC = m_nextPC;
    }
    dumpStatus();
    dumpMem();
}

void CPU::dumpStatus() {
    std::stringstream dump;
    for (uint32_t i = 0; i < REG_SIZE; i++) {
        dump << "r" << i << " : " << m_regFile[i] << "\n";
    }
    emit statusUpd(QString::fromStdString(dump.str()));
}

void CPU::dumpMem() {
    std::stringstream dump;
    for (uint32_t mem = 0; mem < MEM_SIZE; mem++) {
        dump << "0x" << std::setfill('0') << std::setw(4) << std::hex << mem
             << " : " << std::setw(8) << m_mem[mem] << "\n";
    }
    emit memUpd(QString::fromStdString(dump.str()));
}
