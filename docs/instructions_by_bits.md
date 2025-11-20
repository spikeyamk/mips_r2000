# MIPS Instruction Set Tables

---

### **Register Arithmetic, Logic, Shift, Comparison Operations**

| Instruction | Description | 31-26 | 25-21 | 20-16 | 15-11 | 10-6 | 5-0 |
|------------|-------------|-------|-------|-------|-------|------|----|
| add rd, rs, rt | Puts the sum of the integers in the register rs and the register rt into register rd and checks for overflow. | 000000 | rs | rt | rd | 00000 | 100000 |
| addu rd, rs, rt | Puts the sum of the integers in the register rs and the register rt into register rd and does not check for overflow. | 000000 | rs | rt | rd | 00000 | 100001 |
| sub rd, rs, rt | Subtracts the integer in register rt from the integer in register rs, putting the result into register rd and checks for overflow. | 000000 | rs | rt | rd | 00000 | 100010 |
| subu rd, rs, rt | Subtracts the integer in register rt from the integer in register rs, putting the result into register rd and does not check for overflow. | 000000 | rs | rt | rd | 00000 | 100011 |
| and rd, rs, rt | Puts the logical AND of the integers from register rs and rt into register rd. | 000000 | rs | rt | rd | 00000 | 100100 |
| or rd, rs, rt | Puts the logical OR of the integers from register rs and rt into register rd. | 000000 | rs | rt | rd | 00000 | 100101 |
| xor rd, rs, rt | Puts the logical XOR of the integers from register rs and rt into register rd. | 000000 | rs | rt | rd | 00000 | 100110 |
| nor rd, rs, rt | Puts the logical NOR of the integers from register rs and rt into register rd. | 000000 | rs | rt | rd | 00000 | 100111 |
| sllv rd, rt, rs | Performs logical shift of contents of register rt left by amount specified by the lowest order six bits of the value in register rs, putting results into register rd. Vacated bits are filled with 0's. | 000000 | rt | rs | rd | 00000 | 000100 |
| srlv rd, rt, rs | Performs logical shift of contents of register rt right by amount specified by the lowest order six bits of the value in register rs, putting results into register rd. Vacated bits are filled with 0's. | 000000 | rt | rs | rd | 00000 | 000110 |
| srav rd, rt, rs | Performs arithmetic shift of contents of register rt right by amount specified by the lowest order six bits of the value in register rs, putting results into register rd. Vacated bits are filled with replicas of the sign bit. | 000000 | rt | rs | rd | 00000 | 000111 |
| slt rd, rs, rt | If the signed integer in register rs is less than the signed integer in register rt then store 0x00000001 in rd, otherwise 0x00000000. | 000000 | rs | rt | rd | 00000 | 101010 |
| sltu rd, rs, rt | If the unsigned integer in register rs is less than the unsigned integer in register rt then store 0x00000001 in rd, otherwise 0x00000000. | 000000 | rs | rt | rd | 00000 | 101011 |

---

### **Immediate Register Shift Operations**

| Instruction | Description | 31-26 | 25-21 | 20-16 | 15-11 | 10-6 | 5-0 |
|------------|-------------|-------|--------|-------|-------|------|-----|
| sll rd, rs, shamt | Performs logical shift of contents of register rs left by shamt bits, putting results into register rd. Vacated bits are filled with 0's. | 000000 | 00000 | rs | rd | shamt | 000000 |
| srl rd, rs, shamt | Performs logical shift of contents of register rs right by shamt bits, putting results into register rd. Vacated bits are filled with 0's. | 000000 | 00000 | rs | rd | shamt | 000010 |
| sra rd, rs, shamt | Performs arithmetic shift of contents of register rs right by shamt bits, putting results into register rd. Vacated bits are filled with replicas of sign bit. | 000000 | 00000 | rs | rd | shamt | 000011 |

---

### **Immediate Arithmetic, Logic, Comparison Operations**

| Instruction | Description | 31-26 | 25-21 | 20-16 | 15-0 |
|------------|-------------|-------|-------|-------|------|
| addi rd, rs, imm | Sum of register rs and sign-extended immediate imm into rd, check overflow. | 001000 | rs | rd | imm |
| addiu rd, rs, imm | Sum of register rs and zero-extended immediate imm into rd, no overflow check. | 001001 | rs | rd | imm |
| andi rd, rs, imm | Logical AND of register rs and zero-extended immediate imm into rd. | 001100 | rs | rd | imm |
| ori rd, rs, imm | Logical OR of register rs and zero-extended immediate imm into rd. | 001101 | rs | rd | imm |
| xori rd, rs, imm | Logical XOR of register rs and zero-extended immediate imm into rd. | 001110 | rs | rd | imm |
| slti rd, rs, imm | If signed rs < sign-extended imm, store 1 in rd, else 0. | 001010 | rs | rd | imm |
| sltiu rd, rs, imm | If unsigned rs < zero-extended imm, store 1 in rd, else 0. | 001011 | rs | rd | imm |

---

### **Load and Store Operations**

| Instruction | Description | 31-26 | 25-21 | 20-16 | 15-0 |
|------------|-------------|-------|-------|-------|------|
| lui rd, imm | Load immediate imm into upper half of rd, lower bits = 0. | 001111 | 00000 | rd | imm |
| lb rd, imm(rs) | Sign-extend byte at address = rs + imm into rd. | 100000 | rs | rd | imm |
| lbu rd, imm(rs) | Zero-extend byte at address = rs + imm into rd. | 100100 | rs | rd | imm |
| lh rd, imm(rs) | Sign-extend halfword at address = rs + imm into rd. Must be even address. | 100001 | rs | rd | imm |
| lhu rd, imm(rs) | Zero-extend halfword at address = rs + imm into rd. Must be even address. | 100101 | rs | rd | imm |
| lw rd, imm(rs) | Load word at address = rs + imm into rd. Address must be divisible by 4. | 100011 | rs | rd | imm |
| sb rt, imm(rs) | Store low byte of rt at address = rs + imm. | 101000 | rs | rt | imm |
| sh rt, imm(rs) | Store low halfword of rt at address = rs + imm. | 101001 | rs | rt | imm |
| sw rt, imm(rs) | Store word of rt at address = rs + imm. | 101011 | rs | rt | imm |

---

### **Branch Operations**

| Instruction | Description | 31-26 | 25-21 | 20-16 | 15-0 |
|------------|-------------|-------|-------|-------|------|
| bltz rs, label | If rs < 0, PC += imm * 4. | 000001 | rs | 00000 | imm |
| bgez rs, label | If rs >= 0, PC += imm * 4. | 000001 | rs | 00001 | imm |
| bltzal rs, label | If rs < 0, PC += imm * 4 and save next PC in 31. | 000001 | rs | 10000 | imm |
| bgezal rs, label | If rs >= 0, PC += imm * 4 and save next PC in 31. | 000001 | rs | 10001 | imm |
| beq rs, rt, label | If rs == rt, PC += imm * 4. | 000100 | rs | rt | imm |
| bne rs, rt, label | If rs != rt, PC += imm * 4. | 000101 | rs | rt | imm |
| blez rs, label | If rs <= 0, PC += imm * 4. | 000110 | rs | 00000 | imm |
| bgtz rs, label | If rs > 0, PC += imm * 4. | 000111 | rs | 00000 | imm |

---

### **Immediate Jump Operations**

| Instruction | Description | 31-26 | 25-0 |
|------------|-------------|-------|------|
| j target | Unconditional jump to Target * 4. | 000010 | Target |
| jal target | Save next PC in 31, then jump to Target * 4. | 000011 | Target |

---

### **Register Jump Operations**

| Instruction | Description | 31-26 | 25-21 | 20-16 | 15-11 | 10-0 |
|-------------|-------------|-------|-------|-------|-------|------|
| jr rs | Jump to address stored in rs. | 000000 | rs | 00000 | 00000 | 00000001000 |
| jalr rs | Save next PC in 31, then jump to address in rs. | 000000 | rs | 00000 | 11111 | 00000001001 |

---