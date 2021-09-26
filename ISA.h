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

// ISA format:
// Opcode
// Name
// Execute (CPU& cpu)
// AsmArgs (std::stringstream &input) { std::string arg;
// DisasmArgs () std::stringstream args; args << name; ... return args.str();

#define READ_IMM {input >> arg; m_r3_imm = stoi(arg);}
#define WRITE_IMM {args << " " << m_r3_imm;}

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

_ISA(0x01, EXIT, {cpu->m_run = 0;}, {}, {})

_ISA(0x02, B,\
     {cpu->m_nextPC = m_r3_imm;},\
     {READ_IMM},\
     {WRITE_IMM})

// MOV rd <- rs
_ISA(0x10, MOV,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2];},\
    {READ_2REGS},\
    {WRITE_2REGS})

// MOV rd <- imm_low
_ISA(0x11, MOVli,\
    {cpu->m_regFile[m_r1] = (cpu->m_regFile[m_r1] & 0xFF00) + m_r3_imm;},\
    {READ_REG_IMM},\
    {WRITE_REG_IMM})

// MOV rd <- imm_high
_ISA(0x12, MOVhi,\
    {cpu->m_regFile[m_r1] = (cpu->m_regFile[m_r1] & 0xFF) + (m_r3_imm << 16);},\
    {READ_REG_IMM},\
    {WRITE_REG_IMM})

// ADD rd <- rs1 + rs2
_ISA(0x21, ADD,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS},\
    {WRITE_3REGS})

// ADDi rd <- rs + imm
_ISA(0x31, ADDi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + m_r3_imm;},\
    {READ_2REGS_IMM},\
    {WRITE_2REGS_IMM})
