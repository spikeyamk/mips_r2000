#pragma once

template<typename ... Args>
auto cc(const Args& ... args) {
    return (args, ...);
}

struct Constants {
    static constexpr int unsigned REG_COUNT = 32;
};

struct Decode {
    enum LoadStoreDataSizeMode {
        LoadStoreDataSizeMode_BYTE = 0b00,
        LoadStoreDataSizeMode_HALF_WORD = 0b01,
        LoadStoreDataSizeMode_WORD = 0b11
    };

    enum BranchMode {
        BranchMode_BLTZ = 0b000,
        BranchMode_BGEZ = 0b001,
        BranchMode_BEQ = 0b100,
        BranchMode_BNE = 0b101,
        BranchMode_BLEZ = 0b110,
        BranchMode_BGTZ = 0b111
    };

    enum ALUMode {
        ALUMode_ADD = 0b1'0000,
        ALUMode_ADDU = 0b1'0001,
        ALUMode_SUB = 0b1'0010,
        ALUMode_SUBU = 0b1'0011,
        ALUMode_AND = 0b1'0100,
        ALUMode_OR = 0b1'0101,
        ALUMode_XOR = 0b1'0110,
        ALUMode_NOR = 0b1'0111,
        ALUMode_SLL = 0b0'0100,
        ALUMode_SRL = 0b0'0110,
        ALUMode_SRA = 0b0'0111,
        ALUMode_SLT = 0b1'1010,
        ALUMode_SLTU = 0b1'1011
    };
};
