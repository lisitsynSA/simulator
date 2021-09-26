#include "instr.h"
#include "cpu.h"
#include <sstream>
#include <QDebug>

std::map<std::string, uint32_t> Instr::m_instrsDict;

void Instr::executeCode(CPU *cpu, uint32_t code) {
    decode(code);
    execute(cpu);
}

void Instr::execute(CPU *cpu) {
    switch (m_opcode) {
    default:
        qDebug() << "[Error] Wrong opcode";
        break;
#define _ISA(_opcode, _name, _execute, _asmargs, _disasm)\
    case _opcode: { _execute } break;
#include "ISA.h"
#undef _ISA
    }
}

void Instr::assembler(std::stringstream &input) {
    std::string name;
    input >> name;
    m_opcode = m_instrsDict[name];
    std::string arg;
    switch (m_opcode) {
    default:
        // Add labels
        qDebug() << "[Error] Wrong opcode";
        break;
#define _ISA(_opcode, _name, _execute, _asmargs, _disasm)\
    case _opcode: { _asmargs } break;
#include "ISA.h"
#undef _ISA
    }
}

std::string Instr::disasm() {
    std::stringstream args;
    switch (m_opcode) {
    default:
        qDebug() << "[Error] Wrong opcode";
        break;
#define _ISA(_opcode, _name, _execute, _asmargs, _disasm)\
    case _opcode: { _disasm } break;
#include "ISA.h"
#undef _ISA
    }
    return args.str();
}

uint32_t Instr::code() {
    return (m_opcode << 24) | (m_rs1 << 20) | (m_rs1 << 16) | m_rs3_imm;
}

void Instr::decode(uint32_t code) {
    m_opcode = code >> 24;
    m_rs1 = (code >> 20) & 0xF;
    m_rs2 = (code >> 16) & 0xF;
    m_rs3_imm = code & 0xFF;
}
