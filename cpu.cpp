#include "cpu.h"
#include "qdebug.h"
#include <iomanip>
#include <sstream>

CPU::CPU(QObject *parent) : QObject(parent) {
  Instr::prepareDict();
  updateDisplay();
  execThread = new CPUExecThreads(this);
  m_PC = 0;
}

CPU::~CPU() { delete execThread; }

static bool isDigit(std::string &std) {
  if (std::find_if(std.begin(), std.end(), [](unsigned char c) {
        return !std::isdigit(c);
      }) == std.end()) {
    return true;
  }
  return false;
}

std::string CPU::processLabels(std::string input_string,
                               std::stringstream &labels) {
  std::stringstream input;
  input.str(input_string);
  std::map<std::string, uint32_t> label_map;
  Instr instr;
  uint32_t pc = 0;
  while (input.rdbuf()->in_avail()) {
    std::string input_line;
    std::getline(input, input_line);
    if (input_line.empty()) {
      continue;
    }
    std::size_t found = input_line.find(":");
    if (found != std::string::npos) {
      label_map[input_line.substr(0, found)] = pc;
      continue;
    }
    try {
      std::stringstream instr_stream(input_line);
      std::string label = instr.assembler(instr_stream);
      if (label.empty()) {
        pc++;
      } else {
        if (isDigit(label)) {
          showMsg(QString("[ERROR] invalid instruction at ") +
                  QString::number(pc));
          return std::string();
        }
      }
    } catch (std::invalid_argument const &) {
      pc++;
    } catch (...) {
      showMsg(QString("[ERROR] invalid instruction at ") + QString::number(pc));
      return std::string();
    }
  }
  for (auto &labelRec : label_map) {
    std::string label = labelRec.first;
    std::string targetPC = std::to_string(labelRec.second);
    labels << " " << label << "[" << targetPC << "]";
  }
  std::stringstream final_input;
  final_input.str(input_string);
  std::stringstream output;
  pc = 0;
  while (final_input.rdbuf()->in_avail()) {
    std::string input_line;
    std::getline(final_input, input_line);
    if (input_line.empty()) {
      continue;
    }
    std::size_t found = input_line.find(":");
    if (found != std::string::npos) {
      continue;
    }
    std::size_t last_space = input_line.find_last_of(" ") + 1;
    std::string label = input_line.substr(last_space);
    if (label_map.find(label) != label_map.end()) {
      input_line.replace(last_space, label.size(),
                         std::to_string(label_map[label]));
    }
    pc++;
    output << input_line << "\n";
  }

  return output.str();
}

void CPU::readInstrs(QString input_string) {
  if (m_run) {
    return;
  }
  std::stringstream input;
  std::stringstream labels;
  input.str(processLabels(input_string.toStdString(), labels));
  if (!input.rdbuf()->in_avail()) {
    return;
  }
  Instr instr;
  for (uint32_t mem = 0; mem < MEM_SIZE; mem++) {
    if (input.rdbuf()->in_avail()) {
      std::string label;
      try {
        label = instr.assembler(input);
        if (!label.empty()) {
          showMsg(QString("[ERROR] invalid instruction ") +
                  QString::number(mem) + QString(" : ") +
                  QString::fromStdString(label));
          return;
        }
        m_mem[mem] = instr.code();
      } catch (std::invalid_argument const &) {
        showMsg(QString("[ERROR] invalid instruction ") + QString::number(mem));
        return;
      }
    } else {
      m_mem[mem] = 0;
    }
  }
  m_PC = 0;
  dumpStatus();
  dumpMem();
  updateDisplay();
  showMsg(QString("Code loaded. Labels: ") +
          QString::fromStdString(labels.str()));
}

void CPU::run() {
  if (m_run) {
    return;
  }
  m_run = true;
  showMsg("CPU running...");
  execThread->start();
}

void CPU::pause() {
  m_run = false;
  Instr instr;
  instr.decode(m_mem[m_nextPC]);
  showMsg(QString::fromStdString("CPU paused at " + instr.disasm() + " [" +
                                 instr.dumpRegs(this) + "]"));
}

void CPU::stop() {
  m_run = false;
  m_PC = 0;
  showMsg("CPU stoped");
  dumpStatus();
  dumpMem();
}

void CPU::step() {
  if (m_run) {
    return;
  }
  Instr instr;
  instr.decode(m_mem[m_PC]);
  m_nextPC = m_PC + 1;
  instr.execute(this);
  m_PC = m_nextPC;
  instr.decode(m_mem[m_PC]);
  showMsg(QString::fromStdString("CPU paused at " + instr.disasm() + " [" +
                                 instr.dumpRegs(this) + "]"));
  dumpStatus();
  dumpMem();
}

void CPUExecThreads::run() {
  Instr instr;
  while (cpu->m_run) {
    cpu->m_nextPC = cpu->m_PC + 1;
    instr.executeCode(cpu, cpu->m_mem[cpu->m_PC]);
    if (cpu->m_run) {
      cpu->m_PC = cpu->m_nextPC;
    }
  }
  cpu->dumpStatus();
  cpu->dumpMem();
}

void CPU::dumpStatus() {
  std::stringstream dump;
  dump << "PC : " << m_PC << "\n\n";
  for (uint32_t i = 0; i < REG_SIZE; i++) {
    dump << "r" << i << " : " << m_regFile[i] << "\n";
  }
  emit statusUpd(QString::fromStdString(dump.str()));
}

void CPU::dumpMem() {
  std::stringstream dump;
  for (uint32_t mem = 0; mem < MEM_SIZE; mem++) {
    dump << "0x" << std::setfill('0') << std::setw(4) << std::hex << mem
         << " : " << std::setw(2) << (m_mem[mem] >> 24) << " " << std::setw(2)
         << ((m_mem[mem] >> 16) & 0xFF) << " " << std::setw(2)
         << ((m_mem[mem] >> 8) & 0xFF) << " " << std::setw(2)
         << (m_mem[mem] & 0xFF) << "\n";
  }
  emit memUpd(QString::fromStdString(dump.str()));
}

void CPU::updateDisplay() {
  emit displayUpd(m_mem + (MEM_SIZE / 2), DIS_WIDTH, DIS_HEIGHT, DIS_SCALE);
}

void CPU::showMsg(QString msg) { emit sendMsg(msg); }
