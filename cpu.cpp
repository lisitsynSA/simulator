#include "cpu.h"
#include <sstream>
#include <QDebug>
#include <iomanip>

CPU::CPU(QObject *parent) : QObject(parent)
{
    Instr::prepareDict();
    updateDisplay();
}

std::string CPU::processLabels(std::string input_string) {
    std::stringstream instrs;
    std::stringstream input;
    input.str(input_string);
    Instr instr;
    uint32_t pc = 0;
    while (input.rdbuf()->in_avail()) {
        instr.assembler(input);
        pc++;
    }
    return input_string;
}

void CPU::readInstrs(QString input_string){
    std::stringstream input;
    //input.str(processLabels(input_string.toStdString()));
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
        updateDisplay();
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
        dump << "0x" << std::setfill('0') << std::setw(4) << std::hex << mem << " : "
             << std::setw(2) <<  (m_mem[mem] >> 24) << " "
             << std::setw(2) << ((m_mem[mem] >> 16) & 0xFF) << " "
             << std::setw(2) << ((m_mem[mem] >> 8) & 0xFF) << " "
             << std::setw(2) <<  (m_mem[mem] & 0xFF) << "\n";
    }
    emit memUpd(QString::fromStdString(dump.str()));
}

void CPU::updateDisplay() {
    emit displayUpd(m_mem + (MEM_SIZE / 2), DIS_WIDTH, DIS_HEIGHT);
}
