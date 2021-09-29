// Instructions:
// 0x0_ Control flow : EXIT, B, BL, RET, BR, B.EQ, B.NE, B.GT, B.LE
// 0x1_ Moves : MOV, MOVli, MOVhi

// 0x2_ Arithmetic : NEG, ADD, SUB, MUL, DIV, DIV_REM, POW
// 0x3_ Arithmetic w/ imm : ADDi, SUBi, MULi, DIVi, DIV_REMi, POWi, POWi_

// 0x4_ Bitwise : NOT, AND, OR, XOR, SHL, SHR, SHRA
// 0x5_ Bitwise w/ imm : ANDi, ORi, XORi, SHLi, SHRi, SHRAi

// 0x6_ Float Arithmetic : ADDf, SUBf, MULf, DIVf, POWf

// 0x7_ Memory : LD, ST, LDi, STi

// 0x8_ Special : FLUSH, RAND, BKPT

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

///
/// 0x0_ Control flow instructions
///
// EXIT stop CPU
_ISA(0x01, EXIT,\
     {cpu->stop();},\
     {}, {}, {})

// B imm -> nextPC
_ISA(0x02, B,\
     {cpu->m_nextPC = m_r3_imm;},\
     {READ_IMM}, {WRITE_IMM}, {})

// BL r0 = PC+1; imm -> nextPC
_ISA(0x03, BL,\
     {cpu->m_regFile[0] = cpu->m_PC + 1; cpu->m_nextPC = m_r3_imm;},\
     {READ_IMM}, {WRITE_IMM}, {})

// RET r0 -> nextPC
_ISA(0x04, RET,\
     {cpu->m_nextPC = cpu->m_regFile[0];},\
     {}, {}, {})

// BR rs1 -> nextPC
_ISA(0x05, BR,\
     {cpu->m_nextPC = cpu->m_regFile[m_r1];},\
     {READ_REG}, {WRITE_REG}, {DUMP_REG_1})


// B.EQ rs1 == rs2 ? imm -> nextPC
_ISA(0x06, B.EQ,\
     {if (cpu->m_regFile[m_r1] == cpu->m_regFile[m_r2])\
      {cpu->m_nextPC = m_r3_imm;}},\
     {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_2REGS_12})

// B.NE rs1 != rs2 ? imm -> nextPC
_ISA(0x07, B.NE,\
     {if (cpu->m_regFile[m_r1] != cpu->m_regFile[m_r2])\
      {cpu->m_nextPC = m_r3_imm;}},\
     {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_2REGS_12})

// B.GT rs1 > rs2 ? imm -> nextPC
_ISA(0x08, B.GT,\
     {if (cpu->m_regFile[m_r1] > cpu->m_regFile[m_r2])\
      {cpu->m_nextPC = m_r3_imm;}},\
     {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_2REGS_12})

// B.LE rs1 <= rs2 ? imm -> nextPC
_ISA(0x09, B.LE,\
     {if (cpu->m_regFile[m_r1] <= cpu->m_regFile[m_r2])\
      {cpu->m_nextPC = m_r3_imm;}},\
     {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_2REGS_12})

///
/// 0x1_ Move instructions
///
// MOV rd = rs
_ISA(0x10, MOV,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2];},\
    {READ_2REGS}, {WRITE_2REGS}, {DUMP_REG_2})

// MOVli rd_low = imm
_ISA(0x11, MOVli,\
    {cpu->m_regFile[m_r1] = (cpu->m_regFile[m_r1] & 0xFFFF0000) + m_r3_imm;},\
    {READ_REG_IMM}, {WRITE_REG_IMM}, {})

// MOVhi rd_high = imm
_ISA(0x12, MOVhi,\
    {cpu->m_regFile[m_r1] = (cpu->m_regFile[m_r1] & 0xFFFF) + (m_r3_imm << 16);},\
    {READ_REG_IMM}, {WRITE_REG_IMM}, {})

///
/// 0x2_ Arithmetic instructions
///
// NEG rd = -rs
_ISA(0x20, NEG,\
     {cpu->m_regFile[m_r1] = -cpu->m_regFile[m_r2];},\
     {READ_2REGS}, {WRITE_2REGS}, {DUMP_2REGS_12})

// ADD rd = rs1 + rs2
_ISA(0x21, ADD,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// SUB rd = rs1 - rs2
_ISA(0x22, SUB,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] - cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// MUL rd = rs1 * rs2
_ISA(0x23, MUL,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] * cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// DIV rd = rs1 / rs2
_ISA(0x24, DIV,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] / cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})


// DIV_REM rd = rs1 % rs2
_ISA(0x25, DIV_REM,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] % cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// POW rd = pow(rs1, rs2)
_ISA(0x26, POW,\
    {cpu->m_regFile[m_r1] = pow(cpu->m_regFile[m_r2], cpu->m_regFile[m_r3_imm]);},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

///
/// 0x3_ Arithmetic w/ imm instructions
///
// ADDi rd = rs + imm
_ISA(0x31, ADDi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// SUBi rd = rs - imm
_ISA(0x32, SUBi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] - m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// MULi rd = rs * imm
_ISA(0x33, MULi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] * m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// DIVi rd = rs / imm
_ISA(0x34, DIVi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] / m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// DIV_REMi rd = rs % imm
_ISA(0x35, DIV_REMi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] % m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// POWi  rd = pow(rs, imm)
_ISA(0x36, POWi,\
    {cpu->m_regFile[m_r1] = pow(cpu->m_regFile[m_r2], m_r3_imm);},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// POWi_  rd = pow(imm, rs)
_ISA(0x37, POWi_,\
    {cpu->m_regFile[m_r1] = pow(m_r3_imm, cpu->m_regFile[m_r2]);},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

///
/// 0x4_ Bitwise instructions
///
// NOT rd = ~rs1
_ISA(0x40, NOT,\
    {cpu->m_regFile[m_r1] = ~cpu->m_regFile[m_r2];},\
    {READ_2REGS}, {WRITE_2REGS}, {DUMP_2REGS_12})

// AND rd = rs1 & rs2
_ISA(0x41, AND,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] & cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// OR rd = rs1 | rs2
_ISA(0x42, OR,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] | cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// XOR rd = rs1 ^ rs2
_ISA(0x43, XOR,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] ^ cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// SHL rd = rs1 << rs2
_ISA(0x44, SHL,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] << cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// SHR rd = (unsigned) rs1 >> rs2
_ISA(0x45, SHR,\
    {cpu->m_regFile[m_r1] = (uint32_t) cpu->m_regFile[m_r2] >> cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// SHRA  rd = rs1 >> rs2
_ISA(0x46, SHRA,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] >> cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})


///
/// 0x5_ Bitwise w/ imm instructions
///
// ANDi rd = rs & imm
_ISA(0x51, ANDi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] & m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// ORi rd = rs | imm
_ISA(0x52, ORi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] | m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// XORi rd = rs ^ imm
_ISA(0x53, XORi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] ^ m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// SHLi rd = rs << imm
_ISA(0x54, SHLi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] << m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// SHRi rd = (unsigned) rs >> imm
_ISA(0x55, SHRi,\
    {cpu->m_regFile[m_r1] = (uint32_t) cpu->m_regFile[m_r2] >> m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

// SHRAi rd = rs >> imm
_ISA(0x56, SHRAi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] >> m_r3_imm;},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_REG_2})

///
/// 0x6_ Float Arithmetic instructions
///
// ADDf 0x61
// SUBf 0x62
// MULf 0x63
// DIVf 0x64
// POWf 0x65
// ADDf rdf = rs1f + rs2f
_ISA(0x61, ADDf,\
    {cpu->m_regFile[m_r1] = (int32_t)((float)cpu->m_regFile[m_r2] + (float)cpu->m_regFile[m_r3_imm]);},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// SUBf rdf = rs1f - rs2f
_ISA(0x62, SUBf,\
    {cpu->m_regFile[m_r1] = (int32_t)((float)cpu->m_regFile[m_r2] - (float)cpu->m_regFile[m_r3_imm]);},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// MULf rdf = rs1f * rs2f
_ISA(0x63, MULf,\
    {cpu->m_regFile[m_r1] = (int32_t)((float)cpu->m_regFile[m_r2] * (float)cpu->m_regFile[m_r3_imm]);},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// DIVf rdf = rs1f / rs2f
_ISA(0x64, DIVf,\
    {cpu->m_regFile[m_r1] = (int32_t)((float)cpu->m_regFile[m_r2] / (float)cpu->m_regFile[m_r3_imm]);},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// POW rdf = pow(rs1f, rs2f)
_ISA(0x65, POWf,\
    {cpu->m_regFile[m_r1] = (int32_t)(pow((float)cpu->m_regFile[m_r2], (float)cpu->m_regFile[m_r3_imm]));},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

///
/// 0x7_ Memory instructions
///
// LD rd = m_mem[rs1 + rs2]
_ISA(0x70, LD,\
    {cpu->m_regFile[m_r1] = cpu->m_mem[cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm]];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_2REGS_23})

// ST m_mem[rs2 + rs3] = rs1
_ISA(0x71, ST,\
    {cpu->m_mem[cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm]] = cpu->m_regFile[m_r1];},\
    {READ_3REGS}, {WRITE_3REGS}, {DUMP_3REGS})

// LDi rd = m_mem[rs + imm]
_ISA(0x72, LDi,\
    {cpu->m_regFile[m_r1] = cpu->m_mem[cpu->m_regFile[m_r2] + m_r3_imm];},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_2REGS_23})

// STi  m_mem[rs + imm] = rs1
_ISA(0x73, STi,\
    {cpu->m_mem[cpu->m_regFile[m_r2] + m_r3_imm] = cpu->m_regFile[m_r1];},\
    {READ_2REGS_IMM}, {WRITE_2REGS_IMM}, {DUMP_3REGS})

///
/// 0x8_ Special instructions
///
// FLUSH update display
_ISA(0x80, FLUSH,\
     {cpu->updateDisplay(); QThread::usleep(2);},\
     {}, {}, {})

// RAND rd <- random
_ISA(0x81, RAND,\
     {cpu->m_regFile[m_r1] = qrand();},\
     {READ_REG}, {WRITE_REG}, {})

// BKPT pause CPU
_ISA(0x82, BKPT,\
     {cpu->pause();},\
     {}, {}, {})

