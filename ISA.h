// ISA format:
// Opcode
// Name
// Execute (CPU& cpu)
// AsmArgs (std::stringstream &input) { std::string arg;
// Disasm () std::stringstream args; ... return args.str();

_ISA(1, EXIT, {cpu->m_run = 0;}, {}, {args << "EXIT";})

_ISA(2, MOV,\
    {cpu->m_regFile[m_rs1] = cpu->m_regFile[m_rs2];},\
    {input >> arg; m_rs1 = stoi(arg.substr(1));\
     input >> arg; m_rs2 = stoi(arg.substr(1));},\
    {args << "MOV";})

_ISA(3, MOVli,\
    {cpu->m_regFile[m_rs1] = m_rs3_imm;},\
    {input >> arg; m_rs1 = stoi(arg.substr(1));\
     input >> arg; m_rs3_imm = stoi(arg);},\
    {args << "MOVli r" << m_rs1 << " " << m_rs3_imm;})

_ISA(4, MOVhi,\
    {cpu->m_regFile[m_rs1] = m_rs3_imm << 16;},\
    {input >> arg; m_rs1 = stoi(arg.substr(1));\
     input >> arg; m_rs3_imm = stoi(arg);},\
    {args << "MOVhi r" << m_rs1 << " " << m_rs3_imm;})

_ISA(5, ADD,\
    {cpu->m_regFile[m_rs1] = cpu->m_regFile[m_rs2] + cpu->m_regFile[m_rs3_imm];},\
    {input >> arg; m_rs1 = stoi(arg.substr(1));\
     input >> arg; m_rs2 = stoi(arg.substr(1));\
     input >> arg; m_rs3_imm = stoi(arg.substr(1));},\
    {})

_ISA(6, ADDi,\
    {cpu->m_regFile[m_rs1] = cpu->m_regFile[m_rs2] + m_rs3_imm;},\
    {input >> arg; m_rs1 = stoi(arg.substr(1));\
     input >> arg; m_rs2 = stoi(arg.substr(1));\
     input >> arg; m_rs3_imm = stoi(arg);},\
    {})
