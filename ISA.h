// ISA format:
// Opcode
// Name
// Execute (CPU& cpu)
// AsmArgs (std::stringstream &input) { std::string arg;
// DisasmArgs () std::stringstream args; args << name; ... return args.str();

#define READ_2REGS {input >> arg; m_r1 = stoi(arg.substr(1));\
    input >> arg; m_r2 = stoi(arg.substr(1));}
#define WRITE_2REGS {args << " r" << m_r1 << " r" << m_r2;}

#define READ_REG_IMM {input >> arg; m_r1 = stoi(arg.substr(1));\
    input >> arg; m_r3_imm = stoi(arg);}
#define WRITE_REG_IMM {args << " r" << m_r1 << " " << m_r3_imm;}

#define READ_3REGS {input >> arg; m_r1 = stoi(arg.substr(1));\
    input >> arg; m_r2 = stoi(arg.substr(1));\
    input >> arg; m_r3_imm = stoi(arg.substr(1));}
#define WRITE_3REGS {args << " r" << m_r1 << " r" << m_r2 << " r" << m_r3_imm;}

#define READ_2REGS_IMM {input >> arg; m_r1 = stoi(arg.substr(1));\
    input >> arg; m_r2 = stoi(arg.substr(1));\
    input >> arg; m_r3_imm = stoi(arg);}
#define WRITE_2REGS_IMM {args << " r" << m_r1 << " r" << m_r2 << " " << m_r3_imm;}

_ISA(1, EXIT, {cpu->m_run = 0;}, {}, {})

// MOV rd <- rs
_ISA(2, MOV,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2];},\
    {READ_2REGS},\
    {WRITE_2REGS})

// MOV rd <- imm_low
_ISA(3, MOVli,\
    {cpu->m_regFile[m_r1] = (cpu->m_regFile[m_r1] & 0xFF00) + m_r3_imm;},\
    {READ_REG_IMM},\
    {WRITE_REG_IMM})

// MOV rd <- imm_high
_ISA(4, MOVhi,\
    {cpu->m_regFile[m_r1] = (cpu->m_regFile[m_r1] & 0xFF) + (m_r3_imm << 16);},\
    {READ_REG_IMM},\
    {WRITE_REG_IMM})

// ADD rd <- rs1 + rs2
_ISA(5, ADD,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm];},\
    {READ_3REGS},\
    {WRITE_3REGS})

_ISA(6, ADDi,\
    {cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + m_r3_imm;},\
    {READ_2REGS_IMM},\
    {WRITE_2REGS_IMM})
