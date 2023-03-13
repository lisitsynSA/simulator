# CPU simulator with display

## Instr format:
```
FF FF FF FF  
__ __ ^^^^ - r3_imm third reg or imm  
__ _^ __ __ - r2 second reg  
__ ^_ __ __ - r1 first reg  
^^__ __ __ - opcode - operation code 
```
Registers: r0-r15   

## Memory:
```
MEM_SIZE = 65536 words (32 bits)  
0x0000 ↓ CODE + DATA  
...  
0x8000 ↑ STACK  
... DISPLAY MEM (32768 words == 256*128 ARGB)  
0xFFFF  
```

## Interfase
For run CPU place assempler in the left window, `Load code` and `Run CPU`. During execution you can `Pause CPU` and check status in the middle (PC and registers) and right (memory) windows. After pause you can execute one instruction with `Step CPU`, reset with `Stop CPU` and continue execution with `Run CPU`.  
In the lower left window full ISA description is placed.

![image](https://user-images.githubusercontent.com/13435493/224582909-964ae374-835a-4ff6-b3ad-7b39c318b8e3.png)

## Simulator ISA
### 0x01 EXIT
{ cpu->stop(); }
### 0x02 B
{ cpu->m_nextPC = m_r3_imm; }
### 0x03 BL
{ cpu->m_regFile[m_r1] = cpu->m_PC + 1; cpu->m_nextPC = m_r3_imm; }
### 0x04 BR
{ cpu->m_nextPC = cpu->m_regFile[m_r1]; }
### 0x05 B.EQ
{ if (cpu->m_regFile[m_r1] == cpu->m_regFile[m_r2]) { cpu->m_nextPC = m_r3_imm; } }
### 0x06 B.NE
{ if (cpu->m_regFile[m_r1] != cpu->m_regFile[m_r2]) { cpu->m_nextPC = m_r3_imm; } }
### 0x07 B.GT
{ if ((int32_t)cpu->m_regFile[m_r1] > (int32_t)cpu->m_regFile[m_r2]) { cpu->m_nextPC = m_r3_imm; } }
### 0x08 B.LE
{ if ((int32_t)cpu->m_regFile[m_r1] m_regFile[m_r2]) { cpu->m_nextPC = m_r3_imm; } }
### 0x09 B.GTU
{ if (cpu->m_regFile[m_r1] > cpu->m_regFile[m_r2]) { cpu->m_nextPC = m_r3_imm; } }
### 0x10 B.LEU
{ if (cpu->m_regFile[m_r1] m_regFile[m_r2]) { cpu->m_nextPC = m_r3_imm; } }
### 0x11 MOV
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2]; }
### 0x12 MOVli
{ cpu->m_regFile[m_r1] = m_r3_imm; }
### 0x13 MOVhi
{ cpu->m_regFile[m_r1] = m_r3_imm 
### 0x20 NEG
{ cpu->m_regFile[m_r1] = -cpu->m_regFile[m_r2]; }
### 0x21 ADD
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm]; }
### 0x22 SUB
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] - cpu->m_regFile[m_r3_imm]; }
### 0x23 MUL
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] * cpu->m_regFile[m_r3_imm]; }
### 0x24 DIV
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] / cpu->m_regFile[m_r3_imm]; }
### 0x25 DIVrem
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] % cpu->m_regFile[m_r3_imm]; }
### 0x26 POW
{ cpu->m_regFile[m_r1] = pow(cpu->m_regFile[m_r2], cpu->m_regFile[m_r3_imm]); }
### 0x31 ADDi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] + (int16_t)m_r3_imm; }
### 0x32 SUBi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] - (int16_t)m_r3_imm; }
### 0x33 MULi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] * (int16_t)m_r3_imm; }
### 0x34 divi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] / (int16_t)m_r3_imm; }
### 0x35 DIV_REMi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] % (int16_t)m_r3_imm; }
### 0x36 POWi
{ cpu->m_regFile[m_r1] = pow(cpu->m_regFile[m_r2], (int16_t)m_r3_imm); }
### 0x37 POWi_
{ cpu->m_regFile[m_r1] = pow((int16_t)m_r3_imm, cpu->m_regFile[m_r2]); }
### 0x40 NOT
{ cpu->m_regFile[m_r1] = ~cpu->m_regFile[m_r2]; }
### 0x41 AND
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] & cpu->m_regFile[m_r3_imm]; }
### 0x42 OR
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] | cpu->m_regFile[m_r3_imm]; }
### 0x43 XOR
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] ^ cpu->m_regFile[m_r3_imm]; }
### 0x44 SHL
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] m_regFile[m_r3_imm]; }
### 0x45 SHR
{ cpu->m_regFile[m_r1] = (uint32_t)cpu->m_regFile[m_r2] >> cpu->m_regFile[m_r3_imm]; }
### 0x46 SHRA
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] >> cpu->m_regFile[m_r3_imm]; }
### 0x51 ANDi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] & (int16_t)m_r3_imm; }
### 0x52 ORi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] | (int16_t)m_r3_imm; }
### 0x53 XORi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] ^ (int16_t)m_r3_imm; }
### 0x54 SHLi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] 
### 0x55 SHRi
{ cpu->m_regFile[m_r1] = (uint32_t)cpu->m_regFile[m_r2] >> m_r3_imm; }
### 0x56 SHRAi
{ cpu->m_regFile[m_r1] = cpu->m_regFile[m_r2] >> m_r3_imm; }
### 0x61 ADDf
{ cpu->m_regFile[m_r1] = (int32_t)((float)cpu->m_regFile[m_r2] + (float)cpu->m_regFile[m_r3_imm]); }
### 0x62 SUBf
{ cpu->m_regFile[m_r1] = (int32_t)((float)cpu->m_regFile[m_r2] - (float)cpu->m_regFile[m_r3_imm]); }
### 0x63 MULf
{ cpu->m_regFile[m_r1] = (int32_t)((float)cpu->m_regFile[m_r2] * (float)cpu->m_regFile[m_r3_imm]); }
### 0x64 DIVf
{ cpu->m_regFile[m_r1] = (int32_t)((float)cpu->m_regFile[m_r2] / (float)cpu->m_regFile[m_r3_imm]); }
### 0x65 POWf
{ cpu->m_regFile[m_r1] = (int32_t)(pow((float)cpu->m_regFile[m_r2], (float)cpu->m_regFile[m_r3_imm])); }
### 0x70 LD
{ cpu->m_regFile[m_r1] = cpu->readMem(cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm]); }
### 0x71 ST
{ cpu->readMem(cpu->m_regFile[m_r2] + cpu->m_regFile[m_r3_imm]) = cpu->m_regFile[m_r1]; }
### 0x72 LDi
{ cpu->m_regFile[m_r1] = cpu->readMem(cpu->m_regFile[m_r2] + (int16_t)m_r3_imm); }
### 0x73 STi
{ cpu->readMem(cpu->m_regFile[m_r2] + (int16_t)m_r3_imm) = cpu->m_regFile[m_r1]; }
### 0x80 FLUSH
{ cpu->updateDisplay(); QThread::usleep(10); }
### 0x81 RAND
{ cpu->m_regFile[m_r1] = QRandomGenerator::global()->generate(); }
### 0x82 BKPT
{ cpu->pause(); } 
