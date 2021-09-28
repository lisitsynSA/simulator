// Instructions:
// 0x0_ Control flow : EXIT, B, BL, RET, BR, B.EQ, B.NE, B.GT, B.LE
// 0x1_ Moves : MOV, MOVli, MOVhi

// 0x2_ Arithmetic : NEG, ADD, SUB, MUL, DIV, DIV_REM, POW
// 0x3_ Arithmetic w/ imm : ADDi, SUBi, MULi, DIVi, DIV_REMi, POWi

// 0x4_ Bitwise : NOT, AND, OR, XOR, SHL, SHR, SHRA
// 0x5_ Bitwise w/ imm : ANDi, ORi, XORi, SHLi, SHRi, SHRAi

// 0x6_ Float Arithmetic : ADDf, SUBf, MULf, DIVf, POWf
// 0x7_ Float Arithmetic w/ imm : ADDfi, SUBfi, MULfi, DIVfi, POWfi

// 0x8_ Memory : LD, ST, LDi, STi

// 0x9_ Special : FLUSH, RAND

// ISA format:
// Opcode
// Name
// Execute (CPU* cpu)
// AsmArgs (std::stringstream &input) { std::string arg;
// DisasmArgs () std::stringstream args; args << name; ... return args.str();
// DumpRegs (CPU* cpu) std::stringstream args; ... return args.str();

#define READ_IMM {input >> arg; m_r3_imm = stoi(arg);}
#define WRITE_IMM {args << " " << m_r3_imm;}

#define READ_REG {input >> arg; m_r1 = stoi(arg.substr(1));}
#define WRITE_REG {args << " r" << m_r1;}

#define READ_REG_IMM {input >> arg; m_r1 = stoi(arg.substr(1));\
    input >> arg; m_r3_imm = stoi(arg);}
#define WRITE_REG_IMM {args << " r" << m_r1 << " " << m_r3_imm;}

#define READ_2REGS {input >> arg; m_r1 = stoi(arg.substr(1));\
    input >> arg; m_r2 = stoi(arg.substr(1));}
#define WRITE_2REGS {args << " r" << m_r1 << " r" << m_r2;}

#define READ_2REGS_IMM {input >> arg; m_r1 = stoi(arg.substr(1));\
    input >> arg; m_r2 = stoi(arg.substr(1));\
    input >> arg; m_r3_imm = stoi(arg);}
#define WRITE_2REGS_IMM {args << " r" << m_r1 << " r" << m_r2 << " " << m_r3_imm;}

#define READ_3REGS {input >> arg; m_r1 = stoi(arg.substr(1));\
    input >> arg; m_r2 = stoi(arg.substr(1));\
    input >> arg; m_r3_imm = stoi(arg.substr(1));}
#define WRITE_3REGS {args << " r" << m_r1 << " r" << m_r2 << " r" << m_r3_imm;}

#define DUMP_REG_1 {args << " r" << m_r1 << " = " << cpu->m_regFile[m_r1];}
#define DUMP_REG_2 {args << " r" << m_r2 << " = " << cpu->m_regFile[m_r2];}
#define DUMP_2REGS_12 {args << " r" << m_r1 << " = " << cpu->m_regFile[m_r1]\
                            << " r" << m_r2 << " = " << cpu->m_regFile[m_r2];}
#define DUMP_2REGS_23 {args << " r" << m_r2 << " = " << cpu->m_regFile[m_r2]\
                            << " r" << m_r3_imm << " = " << cpu->m_regFile[m_r3_imm];}
#define DUMP_3REGS {args << " r" << m_r1 << " = " << cpu->m_regFile[m_r1]\
                         << " r" << m_r2 << " = " << cpu->m_regFile[m_r2]\
                         << " r" << m_r3_imm << " = " << cpu->m_regFile[m_r3_imm];}

// EXIT run = 0
_ISA(0x01, EXIT, {cpu->m_run = 0;}, {}, {}, {})

// B imm -> nextPC
_ISA(0x02, B,\
     {cpu->m_nextPC = m_r3_imm;},\
     {READ_IMM},\
     {WRITE_IMM}, {})

// BL 0x03
// RET 0x04
// BR 0x05

// B.EQ rs1 == rs2 ? imm -> nextPC
_ISA(0x06, B.EQ,\
     {if (cpu->m_regFile[m_r1] == cpu->m_regFile[m_r2])\
      {cpu->m_nextPC = m_r3_imm;}},\
     {READ_2REGS_IMM},\
     {WRITE_2REGS_IMM},\
     {DUMP_2REGS_12})

// B.NE 0x07
// B.GT 0x08
// B.LE 0x09

// MOV rd <- rs
_ISA(0x10, MOV,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2];},\
    {READ_2REGS},\
    {WRITE_2REGS},\
    {DUMP_REG_2})

// MOVli rd_low <- imm
_ISA(0x11, MOVli,\
    {cpu->m_regFile[m_r1] = (cpu->m_regFile[m_r1] & 0xFFFF0000) + m_r3_imm;},\
    {READ_REG_IMM},\
    {WRITE_REG_IMM}, {})

// MOVhi rd_high <- imm
_ISA(0x12, MOVhi,\
    {cpu->m_regFile[m_r1] = (cpu->m_regFile[m_r1] & 0xFFFF) + (m_r3_imm << 16);},\
    {READ_REG_IMM},\
    {WRITE_REG_IMM}, {})

// NEG 0x20

// ADD rd <- rs1 + rs2
_ISA(0x21, ADD,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS},\
    {WRITE_3REGS},\
    {DUMP_2REGS_23})

// SUB rd <- rs1 - rs2
_ISA(0x22, SUB,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] - cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS},\
    {WRITE_3REGS},\
    {DUMP_2REGS_23})

// MUL 0x23
// DIV 0x24
// DIV_REM 0x25
// POW 0x26

// ADDi rd <- rs + imm
_ISA(0x31, ADDi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + m_r3_imm;},\
    {READ_2REGS_IMM},\
    {WRITE_2REGS_IMM},\
    {DUMP_REG_2})

// SUBi rd <- rs - imm
_ISA(0x32, SUBi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] - m_r3_imm;},\
    {READ_2REGS_IMM},\
    {WRITE_2REGS_IMM},\
    {DUMP_REG_2})

// MULi 0x33
// DIVi 0x34
// DIV_REMi 0x35
// POWi 0x36

// NOT 0x40
// AND 0x41
// OR 0x42

// XOR rd = rs1 ^ rs2
_ISA(0x43, XOR,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] ^ cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS},\
    {WRITE_3REGS},\
    {DUMP_2REGS_23})

// SHL 0x44
// SHR 0x45
// SHRA 0x46

// ANDi 0x51
// ORi 0x52
// XORi 0x53
// SHLi 0x54
// SHRi 0x55
// SHRAi 0x56

// ADDf 0x61
// SUBf 0x62
// MULf 0x63
// DIVf 0x64
// POWf 0x65

// ADDfi 0x71
// SUBfi 0x72
// MULfi 0x73
// DIVfi 0x74
// POWfi 0x75

// LD rd <- m_mem[rs1 + rs2]
_ISA(0x80, LD,\
    {cpu->m_regFile[m_r1] = cpu->m_mem[cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm]];},\
    {READ_3REGS},\
    {WRITE_3REGS},\
    {DUMP_2REGS_23})

// ST m_mem[rs2 + rs3] <- rs1
_ISA(0x81, ST,\
    {cpu->m_mem[cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm]] = cpu->m_regFile[m_r1];},\
    {READ_3REGS},\
    {WRITE_3REGS},\
    {DUMP_3REGS})

// LDi rd <- m_mem[rs + imm]
_ISA(0x82, LDi,\
    {cpu->m_regFile[m_r1] = cpu->m_mem[cpu->m_regFile[m_r2] + m_r3_imm];},\
    {READ_2REGS_IMM},\
    {WRITE_2REGS_IMM},\
    {DUMP_2REGS_23})

// STi  m_mem[rs + imm] <- rs1
_ISA(0x83, STi,\
    {cpu->m_mem[cpu->m_regFile[m_r2] + m_r3_imm] = cpu->m_regFile[m_r1];},\
    {READ_2REGS_IMM},\
    {WRITE_2REGS_IMM},\
    {DUMP_3REGS})

// FLUSH
_ISA(0x90, FLUSH, {cpu->updateDisplay(); QThread::usleep(2);}, {}, {}, {})

// RAND rd <- random
_ISA(0x91, RAND, {cpu->m_regFile[m_r1] = qrand();},\
     {READ_REG},\
     {WRITE_REG}, {})
