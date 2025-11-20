## R-Type Instruction Format

| 31-26 | 25-21 | 20-16 | 15-11 | 10-6 | 5-0 |
|:------|:------|:------|:------|:-----|:----|
| opcode (000000) | rs | rt | rd | shamt | funct |

---

## I-Type Instruction Format (ALU Immediate)

| 31-26 | 25-21 | 20-16 | 15-0 |
|:------|:------|:------|:----|
| opcode | rs | rt | imm |

---

## S-Type Instruction Format (Store)

| 31-26 | 25-21 | 20-16 | 15-0 |
|:------|:------|:------|:----|
| opcode | rs | rt | Offset |

---

## L-Type Instruction Format (Load)

| 31-26 | 25-21 | 20-16 | 15-0 |
|:------|:------|:------|:----|
| opcode | rs | rt | Offset |

---

## B-Type Instruction Format (Branch)

| 31-26 | 25-21 | 20-16 | 15-0 |
|:------|:------|:------|:----|
| opcode | rs | rt | Offset |

---

## J-Type Instruction Format

| 31-26 | 25-0 |
|:------|:------|
| opcode | target |