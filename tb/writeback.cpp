#include <memory>
#include <systemc>
#include <ranges>
#include <csignal>
#include <vector>
#include <verilated.h>
#include <verilated_fst_sc.h>
#include "Vmips_r2000.h"
#include "util.hpp"

using namespace sc_core;
using namespace sc_dt;

struct Predictor {
    sc_bv<32> pc_wb { 0 };
    bool rd_wb { 0 };
    sc_bv<5> rd_address_wb { 0 };
    sc_bv<32> rd_data_wb { 0 };

    void operator==(const std::unique_ptr<Vmips_r2000>& dut) const {
        assert(pc_wb == dut->pc_wb.read());
        assert(rd_wb == dut->rd_wb.read());
        assert(rd_address_wb == dut->rd_address_wb.read());
        assert(rd_data_wb == dut->rd_data_wb.read());
    }
};

const Predictor predictor[] {
    {
        // add      $25,$15,$2
        .pc_wb = 772,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115 + 102),
    },
    {
        // addu     $25,$15,$2
        .pc_wb = 776,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115 + 102),
    },
    {
        // sub      $25,$15,$2
        .pc_wb = 780,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115 - 102),
    },
    {
        // subu     $25,$15,$2
        .pc_wb = 784,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115U - 102U),
    },
    {
        // and      $25,$15,$2
        .pc_wb = 788,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115U & 102U),
    },
    {
        // or       $25,$15,$2
        .pc_wb = 792,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115U | 102U),
    },
    {
        // xor      $25,$15,$2
        .pc_wb = 796,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115U ^ 102U),
    },
    {
        // nor      $25,$15,$2
        .pc_wb = 800,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(~(115U | 102U)),
    },
    {
        // sllv     $25,$15,$2
        .pc_wb = 804,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = (115U << (102U & 0b1'1111U)),
    },
    {
        // srlv     $25,$15,$2
        .pc_wb = 808,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = (115U >> (102U & 0b1'1111U)),
    },
    {
        // srav     $25,$15,$2
        .pc_wb = 812,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = (int32_t(115) >> (102U & 0b1'1111U)),
    },
    {
        // slt      $25,$15,$2
        .pc_wb = 816,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115 < 102),
    },
    {
        // sltu     $25,$15,$2
        .pc_wb = 820,
        .rd_wb = 1,
        .rd_address_wb = 25,
        .rd_data_wb = uint32_t(115U < 102U),
    },
    {
        // sll      $15,$2,0x1e
        .pc_wb = 824,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102U << 0x1eU),
    },
    {
        // srl      $15,$2,0x1e
        .pc_wb = 828,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102U >> 0x1eU),
    },
    {
        // sra      $15,$2,0x1e
        .pc_wb = 832,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102U >> 0x1eU),
    },
    {
        // addi     $15,$2,-32000
        .pc_wb = 836,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102 + (-32000)),
    },
    {
        // addiu    $15,$2,-32000
        .pc_wb = 840,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102 + (-32000)),
    },
    {
        // slti     $15,$2,-32000
        .pc_wb = 844,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102 < (-32000)),
    },
    {
        // sltiu    $15,$2,-32000
        .pc_wb = 848,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102 < uint16_t(-32000)),
    },
    {
        // andi     $15,$2,0x7d00
        .pc_wb = 852,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102 & 0x7d00U),
    },
    {
        // ori     $15,$2,0x7d00
        .pc_wb = 856,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102 | 0x7d00U),
    },
    {
        // xori     $15,$2,0x7d00
        .pc_wb = 860,
        .rd_wb = 1,
        .rd_address_wb = 15,
        .rd_data_wb = uint32_t(102 ^ 0x7d00U),
    },
    {
        // lui      $14,0x7d00
        .pc_wb = 864,
        .rd_wb = 1,
        .rd_address_wb = 14,
        .rd_data_wb = uint32_t(0x7d00U << 16U),
    },

    {
        // lui	$17,0xcece
        .pc_wb = 868,
        .rd_wb = 1,
        .rd_address_wb = 17,
        .rd_data_wb = uint32_t(0xceceU << 16U),
    },
    {
        // ori	$17,$17,0xbaba
        .pc_wb = 872,
        .rd_wb = 1,
        .rd_address_wb = 17,
        .rd_data_wb = uint32_t(0xceceU << 16U) | 0xbabaU,
    },
    {
        // sb	$17,0(zero)
        .pc_wb = 876,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // lb	$10,0(zero)
        .pc_wb = 880,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = (
            sc_bv<24>((0xbaU & 0x80u) ? 0xffffffu : 0x000000u),
            sc_bv<8>(0xbaU)
        ),
    },
    {
        // lbu	$10,0(zero)
        .pc_wb = 884,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbaU,
    },
    {
        // lh	$10,0(zero)
        .pc_wb = 888,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbaU,
    },
    {
        // lhu	$10,0(zero)
        .pc_wb = 892,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbaU,
    },
    {
        // lw	$10,0(zero)
        .pc_wb = 896,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbaU,
    },

    {
        // lui	$17,0xcece
        .pc_wb = 900,
        .rd_wb = 1,
        .rd_address_wb = 17,
        .rd_data_wb = uint32_t(0xceceU << 16U),
    },
    {
        // ori	$17,$17,0xbaba
        .pc_wb = 904,
        .rd_wb = 1,
        .rd_address_wb = 17,
        .rd_data_wb = uint32_t(0xceceU << 16U) | 0xbabaU,
    },
    {
        // sh	$17,0(zero)
        .pc_wb = 908,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // lb	$10,0(zero)
        .pc_wb = 912,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = (
            sc_bv<24>((0xbaU & 0x80u) ? 0xffffffu : 0x000000u),
            sc_bv<8>(0xbaU)
        ),
    },
    {
        // lbu	$10,0(zero)
        .pc_wb = 916,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbaU,
    },
    {
        // lh	$10,0(zero)
        .pc_wb = 920,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = (
            sc_bv<16>((0xbabaU & 0x80u) ? 0xffffu : 0x0000u),
            sc_bv<16>(0xbabaU)
        ),
    },
    {
        // lhu	$10,0(zero)
        .pc_wb = 924,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbabaU,
    },
    {
        // lw	$10,0(zero)
        .pc_wb = 928,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbabaU,
    },

    {
        // lui	$17,0xcece
        .pc_wb = 932,
        .rd_wb = 1,
        .rd_address_wb = 17,
        .rd_data_wb = uint32_t(0xceceU << 16U),
    },
    {
        // ori	$17,$17,0xbaba
        .pc_wb = 936,
        .rd_wb = 1,
        .rd_address_wb = 17,
        .rd_data_wb = uint32_t(0xceceU << 16U) | 0xbabaU,
    },
    {
        // sw	$17,0(zero)
        .pc_wb = 940,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // lb	$10,0(zero)
        .pc_wb = 944,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = (
            sc_bv<24>((0xbaU & 0x80u) ? 0xffffffu : 0x000000u),
            sc_bv<8>(0xbaU)
        ),
    },
    {
        // lbu	$10,0(zero)
        .pc_wb = 948,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbaU,
    },
    {
        // lh	$10,0(zero)
        .pc_wb = 952,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = (
            sc_bv<16>((0xbabaU & 0x80u) ? 0xffffu : 0x0000u),
            sc_bv<16>(0xbabaU)
        ),
    },
    {
        // lhu	$10,0(zero)
        .pc_wb = 956,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xbabaU,
    },
    {
        // lw	$10,0(zero)
        .pc_wb = 960,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 0xcecebabaU,
    },

    {
        // beq	    $14,$10,1 <main+0x10>
        .pc_wb = 964,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // bne	    $10,$10,-3 <main+0x8>
        .pc_wb = 968,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // blez	$14,1 <main+0x20>
        .pc_wb = 972,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // bgtz     $14,-3 <main+0x128>
        .pc_wb = 976,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // bltz     $14,5 <main+0x140>
        .pc_wb = 980,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // bgez     $11,-7 <main+0x138>
        .pc_wb = 984,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // bltzal   $11,5 <main+0x138>
        .pc_wb = 988,
        .rd_wb = 0,
        .rd_address_wb = 31,
        .rd_data_wb = 988 + 4 + 4,
    },
    {
        // bgezal   $1,-9 <main+0x130>
        .pc_wb = 992,
        .rd_wb = 0,
        .rd_address_wb = 31,
        .rd_data_wb = 992 + 4 + 4,
    },
    {
        // j        1012
        .pc_wb = 996,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_wb = 1000,
        .rd_wb = 1,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // jal      1028 (896 / 4 = 112)
        .pc_wb = 1012,
        .rd_wb = 1,
        .rd_address_wb = 31,
        .rd_data_wb = 1012 + 4 + 4,
    },
    {
        // ori	$10,$0,1044
        .pc_wb = 1016,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 1044,
    },
    {
        // jr       $10
        .pc_wb = 1028,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // ori	$10,$0,1060
        .pc_wb = 1032,
        .rd_wb = 1,
        .rd_address_wb = 10,
        .rd_data_wb = 1060,
    },
    {
        // jalr     $10
        .pc_wb = 1044,
        .rd_wb = 1,
        .rd_address_wb = 31,
        .rd_data_wb = 1044 + 4 + 4,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_wb = 1048,
        .rd_wb = 1,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_wb = 1060,
        .rd_wb = 1,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_wb = 1064,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_wb = 1068,
        .rd_wb = 1,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_wb = 1064,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_wb = 1068,
        .rd_wb = 1,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_wb = 1064,
        .rd_wb = 0,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_wb = 1068,
        .rd_wb = 1,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    },
};

VerilatedFstSc* tfp = nullptr;

int sc_main(int argc, char* argv[]) {
    Verilated::debug(0);
    Verilated::randReset(2);
    Verilated::traceEverOn(true);
    Verilated::commandArgs(argc, argv);
    std::ios::sync_with_stdio();

    // inputs
    sc_clock clk{ "clk", sc_time { 10.0, SC_NS }, 0.5, sc_time { 3.0, SC_NS } };
    sc_signal<bool> rst;
    const std::vector<uint8_t> ROM {
        // addi $0, $0, 0 type instructions for writeback check
        0x20,0x00,0x00,0x00, // addi	zero,zero,0
        0x20,0x01,0xff,0x9b, // addi	at,zero,-101
        0x20,0x02,0x00,0x66, // addi	v0,zero,102
        0x20,0x03,0x00,0x67, // addi	v1,zero,103
        0x20,0x04,0x00,0x68, // addi	a0,zero,104
        0x20,0x05,0x00,0x69, // addi	a1,zero,105
        0x20,0x06,0x00,0x6a, // addi	a2,zero,106
        0x20,0x07,0x00,0x6b, // addi	a3,zero,107
        0x20,0x08,0x00,0x6c, // addi	t0,zero,108
        0x20,0x09,0x00,0x6d, // addi	t1,zero,109
        0x20,0x0a,0x00,0x6e, // addi	t2,zero,110
        0x20,0x0b,0x00,0x6f, // addi	t3,zero,111
        0x20,0x0c,0x00,0x70, // addi	t4,zero,112
        0x20,0x0d,0x00,0x71, // addi	t5,zero,113
        0x20,0x0e,0x00,0x72, // addi	t6,zero,114
        0x20,0x0f,0x00,0x73, // addi	t7,zero,115
        0x20,0x10,0x00,0x74, // addi	s0,zero,116
        0x20,0x11,0x00,0x75, // addi	s1,zero,117
        0x20,0x12,0x00,0x76, // addi	s2,zero,118
        0x20,0x13,0x00,0x77, // addi	s3,zero,119
        0x20,0x14,0x00,0x78, // addi	s4,zero,120
        0x20,0x15,0x00,0x79, // addi	s5,zero,121
        0x20,0x16,0x00,0x7a, // addi	s6,zero,122
        0x20,0x17,0x00,0x7b, // addi	s7,zero,123
        0x20,0x18,0x00,0x7c, // addi	t8,zero,124
        0x20,0x19,0x00,0x7d, // addi	t9,zero,125
        0x20,0x1a,0x00,0x7e, // addi	k0,zero,126
        0x20,0x1b,0x00,0x7f, // addi	k1,zero,127
        0x20,0x1c,0x00,0x80, // addi	gp,zero,128
        0x20,0x1d,0x00,0x81, // addi	sp,zero,129
        0x20,0x1e,0x00,0x82, // addi	s8,zero,130
        0x20,0x1f,0x00,0x83, // addi	ra,zero,131

        // asm("ori $1,$1,0"); type instructions for writeback check
        0x34,0x00,0x00,0x00,
        0x34,0x21,0x00,0x00,
        0x34,0x42,0x00,0x00,
        0x34,0x63,0x00,0x00,
        0x34,0x84,0x00,0x00,
        0x34,0xa5,0x00,0x00,
        0x34,0xc6,0x00,0x00,
        0x34,0xe7,0x00,0x00,
        0x35,0x08,0x00,0x00,
        0x35,0x29,0x00,0x00,
        0x35,0x4a,0x00,0x00,
        0x35,0x6b,0x00,0x00,
        0x35,0x8c,0x00,0x00,
        0x35,0xad,0x00,0x00,
        0x35,0xce,0x00,0x00,
        0x35,0xef,0x00,0x00,
        0x36,0x10,0x00,0x00,
        0x36,0x31,0x00,0x00,
        0x36,0x52,0x00,0x00,
        0x36,0x73,0x00,0x00,
        0x36,0x94,0x00,0x00,
        0x36,0xb5,0x00,0x00,
        0x36,0xd6,0x00,0x00,
        0x36,0xf7,0x00,0x00,
        0x37,0x18,0x00,0x00,
        0x37,0x39,0x00,0x00,
        0x37,0x5a,0x00,0x00,
        0x37,0x7b,0x00,0x00,
        0x37,0x9c,0x00,0x00,
        0x37,0xbd,0x00,0x00,
        0x37,0xde,0x00,0x00,
        0x37,0xff,0x00,0x00,

        0x20,0x00,0x00,0x00, // addi	zero,zero,0
        0xad,0x40,0xff,0x92, // sw	zero,-110(t2)
        0x20,0x20,0xff,0x9c, // addi	zero,at,-100
        0xad,0x40,0xff,0x96, // sw	zero,-106(t2)
        0x20,0x40,0x00,0x64, // addi	zero,v0,100
        0xad,0x40,0xff,0x9a, // sw	zero,-102(t2)
        0x20,0x60,0x00,0x64, // addi	zero,v1,100
        0xad,0x40,0xff,0x9e, // sw	zero,-98(t2)
        0x20,0x80,0x00,0x64, // addi	zero,a0,100
        0xad,0x40,0xff,0xa2, // sw	zero,-94(t2)
        0x20,0xa0,0x00,0x64, // addi	zero,a1,100
        0xad,0x40,0xff,0xa6, // sw	zero,-90(t2)
        0x20,0xc0,0x00,0x64, // addi	zero,a2,100
        0xad,0x40,0xff,0xaa, // sw	zero,-86(t2)
        0x20,0xe0,0x00,0x64, // addi	zero,a3,100
        0xad,0x40,0xff,0xae, // sw	zero,-82(t2)
        0x21,0x00,0x00,0x64, // addi	zero,t0,100
        0xad,0x40,0xff,0xb2, // sw	zero,-78(t2)
        0x21,0x20,0x00,0x64, // addi	zero,t1,100
        0xad,0x40,0xff,0xb6, // sw	zero,-74(t2)
        0x21,0x40,0x00,0x64, // addi	zero,t2,100
        0xad,0x40,0xff,0xba, // sw	zero,-70(t2)
        0x21,0x60,0x00,0x64, // addi	zero,t3,100
        0xad,0x40,0xff,0xbe, // sw	zero,-66(t2)
        0x21,0x80,0x00,0x64, // addi	zero,t4,100
        0xad,0x40,0xff,0xc2, // sw	zero,-62(t2)
        0x21,0xa0,0x00,0x64, // addi	zero,t5,100
        0xad,0x40,0xff,0xc6, // sw	zero,-58(t2)
        0x21,0xc0,0x00,0x64, // addi	zero,t6,100
        0xad,0x40,0xff,0xca, // sw	zero,-54(t2)
        0x21,0xe0,0x00,0x64, // addi	zero,t7,100
        0xad,0x40,0xff,0xce, // sw	zero,-50(t2)
        0x22,0x00,0x00,0x64, // addi	zero,s0,100
        0xad,0x40,0xff,0xd2, // sw	zero,-46(t2)
        0x22,0x20,0x00,0x64, // addi	zero,s1,100
        0xad,0x40,0xff,0xd6, // sw	zero,-42(t2)
        0x22,0x40,0x00,0x64, // addi	zero,s2,100
        0xad,0x40,0xff,0xda, // sw	zero,-38(t2)
        0x22,0x60,0x00,0x64, // addi	zero,s3,100
        0xad,0x40,0xff,0xde, // sw	zero,-34(t2)
        0x22,0x80,0x00,0x64, // addi	zero,s4,100
        0xad,0x40,0xff,0xe2, // sw	zero,-30(t2)
        0x22,0xa0,0x00,0x64, // addi	zero,s5,100
        0xad,0x40,0xff,0xe6, // sw	zero,-26(t2)
        0x22,0xc0,0x00,0x64, // addi	zero,s6,100
        0xad,0x40,0xff,0xea, // sw	zero,-22(t2)
        0x22,0xe0,0x00,0x64, // addi	zero,s7,100
        0xad,0x40,0xff,0xee, // sw	zero,-18(t2)
        0x23,0x00,0x00,0x64, // addi	zero,t8,100
        0xad,0x40,0xff,0xf2, // sw	zero,-14(t2)
        0x23,0x20,0x00,0x64, // addi	zero,t9,100
        0xad,0x40,0xff,0xf6, // sw	zero,-10(t2)
        0x23,0x40,0x00,0x64, // addi	zero,k0,100
        0xad,0x40,0xff,0xfa, // sw	zero,-6(t2)
        0x23,0x60,0x00,0x64, // addi	zero,k1,100
        0xad,0x40,0xff,0xfe, // sw	zero,-2(t2)
        0x23,0x80,0x00,0x64, // addi	zero,gp,100
        0xad,0x40,0x00,0x02, // sw	zero,2(t2)
        0x23,0xa0,0x00,0x64, // addi	zero,sp,100
        0xad,0x40,0x00,0x06, // sw	zero,6(t2)
        0x23,0xc0,0x00,0x64, // addi	zero,s8,100
        0xad,0x40,0x00,0x0a, // sw	zero,10(t2)
        0x23,0xe0,0x00,0x64, // addi	zero,ra,100
        0xad,0x40,0x00,0x0e, // sw	zero,14(t2)

        0x00,0x00,0x00,0x26, // xor	zero,zero,zero

        0x8c,0x00,0x00,0x00, // lw	zero,0(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x04, // lw	zero,4(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x08, // lw	zero,8(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x0c, // lw	zero,12(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x10, // lw	zero,16(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x14, // lw	zero,20(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x18, // lw	zero,24(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x1c, // lw	zero,28(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x20, // lw	zero,32(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x24, // lw	zero,36(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x28, // lw	zero,40(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x2c, // lw	zero,44(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x30, // lw	zero,48(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x34, // lw	zero,52(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x38, // lw	zero,56(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x3c, // lw	zero,60(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x40, // lw	zero,64(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x44, // lw	zero,68(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x48, // lw	zero,72(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x4c, // lw	zero,76(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x50, // lw	zero,80(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x54, // lw	zero,84(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x58, // lw	zero,88(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x5c, // lw	zero,92(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x60, // lw	zero,96(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x64, // lw	zero,100(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x68, // lw	zero,104(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x6c, // lw	zero,108(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x70, // lw	zero,112(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x74, // lw	zero,116(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x78, // lw	zero,120(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero
        0x8c,0x00,0x00,0x7c, // lw	zero,124(zero)
        0x00,0x00,0x00,0x26, // xor	zero,zero,zero

        0x01,0xe2,0xc8,0x20, // add      $25,$15,$2
        0x01,0xe2,0xc8,0x21, // addu     $25,$15,$2
        0x01,0xe2,0xc8,0x22, // sub      $25,$15,$2
        0x01,0xe2,0xc8,0x23, // subu     $25,$15,$2
        0x01,0xe2,0xc8,0x24, // and      $25,$15,$2
        0x01,0xe2,0xc8,0x25, // or       $25,$15,$2
        0x01,0xe2,0xc8,0x26, // xor      $25,$15,$2
        0x01,0xe2,0xc8,0x27, // nor      $25,$15,$2
        0x00,0x4f,0xc8,0x04, // sllv     $25,$15,$2
        0x00,0x4f,0xc8,0x06, // srlv     $25,$15,$2
        0x00,0x4f,0xc8,0x07, // srav     $25,$15,$2
        0x01,0xe2,0xc8,0x2a, // slt      $25,$15,$2
        0x01,0xe2,0xc8,0x2b, // sltu     $25,$15,$2
        0x00,0x02,0x7f,0x80, // sll      $15,$2,0x1e
        0x00,0x02,0x7f,0x82, // srl      $15,$2,0x1e
        0x00,0x02,0x7f,0x83, // sra      $15,$2,0x1e
        0x20,0x4f,0x83,0x00, // addi     $15,$2,-32000
        0x24,0x4f,0x83,0x00, // addiu    $15,$2,-32000
        0x28,0x4f,0x83,0x00, // slti     $15,$2,-32000
        0x2c,0x4f,0x83,0x00, // sltiu    $15,$2,-32000
        0x30,0x4f,0x7d,0x00, // andi     $15,$2,0x7d00
        0x34,0x4f,0x7d,0x00, // ori      $15,$2,0x7d00
        0x38,0x4f,0x7d,0x00, // xori     $15,$2,0x7d00
        0x3c,0x0e,0x7d,0x00, // lui      $14,0x7d00

        0x3c,0x11,0xce,0xce, // lui	$17,0xcece
        0x36,0x31,0xba,0xba, // ori	$17,$17,0xbaba
        0xa0,0x11,0x00,0x00, // sb	$17,0(zero)
        0x80,0x0a,0x00,0x00, // lb	$10,0(zero)
        0x90,0x0a,0x00,0x00, // lbu	$10,0(zero)
        0x84,0x0a,0x00,0x00, // lh	$10,0(zero)
        0x94,0x0a,0x00,0x00, // lhu	$10,0(zero)
        0x8c,0x0a,0x00,0x00, // lw	$10,0(zero)

        0x3c,0x11,0xce,0xce, // lui	$17,0xcece
        0x36,0x31,0xba,0xba, // ori	$17,$17,0xbaba
        0xa4,0x11,0x00,0x00, // sh	$17,0(zero)
        0x80,0x0a,0x00,0x00, // lb	$10,0(zero)
        0x90,0x0a,0x00,0x00, // lbu	$10,0(zero)
        0x84,0x0a,0x00,0x00, // lh	$10,0(zero)
        0x94,0x0a,0x00,0x00, // lhu	$10,0(zero)
        0x8c,0x0a,0x00,0x00, // lw	$10,0(zero)

        0x3c,0x11,0xce,0xce, // lui	$17,0xcece
        0x36,0x31,0xba,0xba, // ori	$17,$17,0xbaba
        0xac,0x11,0x00,0x00, // sw	$17,0(zero)
        0x80,0x0a,0x00,0x00, // lb	$10,0(zero)
        0x90,0x0a,0x00,0x00, // lbu	$10,0(zero)
        0x84,0x0a,0x00,0x00, // lh	$10,0(zero)
        0x94,0x0a,0x00,0x00, // lhu	$10,0(zero)
        0x8c,0x0a,0x00,0x00, // lw	$10,0(zero)

        0x11,0xca,0x00,0x01, // beq	    $14,$10,1 <main+0x10>
        0x15,0x4a,0xff,0xfd, // bne	    $10,$10,-3 <main+0x8>
        0x19,0xc0,0x00,0x01, // blez	$14,1 <main+0x20>
        0x1c,0x20,0xff,0xfd, // bgtz	$1,-3 <main+0x18>
        0x05,0xc0,0x00,0x05, // bltz	$14,5 <main+0x40>
        0x04,0x21,0xff,0xf9, // bgez	$1,-7 <main+0x18>
        0x05,0x70,0x00,0x05, // bltzal	$11,5 <main+0x50>
        0x04,0x31,0xff,0xf9, // bgezal	$1,-9 <main+0x28>

        0x08,0x00,0x00,0xfd, // j	1012 <main+0x3f4>
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x0c,0x00,0x01,0x01, // jal	1028 <main+0x404>
        0x34,0x0a,0x04,0x14, // ori	$10,$0,1044
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x01,0x40,0x00,0x08, // jr	$10
        0x34,0x0a,0x04,0x24, // ori	$10,$0,1060
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x01,0x40,0xf8,0x09, // jalr	$10
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x10,0x00,0xff,0xff, // beq	$0,$0,-1
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle

        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
    };

    assert((ROM.size() > 4) && ((ROM.size() % 4) == 0));
    std::vector<sc_signal<sc_bv<8>>> rom(std::extent_v<std::remove_reference_t<decltype(Vmips_r2000::rom)>>);
    sc_signal<bool> stall;

    // outputs
    sc_signal<sc_bv<32>> pc_wb;
    std::vector<sc_signal<sc_bv<8>>> ram(std::extent_v<std::remove_reference_t<decltype(Vmips_r2000::ram)>>);
    std::vector<sc_signal<sc_bv<32>>> reg_file(std::extent_v<std::remove_reference_t<decltype(Vmips_r2000::reg_file)>>);
    sc_signal<bool> rd_wb;
    sc_signal<sc_bv<5>> rd_address_wb;
    sc_signal<sc_bv<32>> rd_data_wb;

    const std::unique_ptr<Vmips_r2000> dut{new Vmips_r2000{"writeback_context"}};

    // inputs
    dut->clk(clk);
    dut->rst(rst);
    for(const auto& [port, sig]: std::views::zip(dut->rom, rom)) {
        port(sig);
    }
    dut->stall(stall);

    // outputs
    dut->pc_wb(pc_wb);
    for(const auto& [port, sig]: std::views::zip(dut->ram, ram)) {
        port(sig);
    }
    for(const auto& [port, sig]: std::views::zip(dut->reg_file, reg_file)) {
        port(sig);
    }
    dut->rd_wb(rd_wb);
    dut->rd_address_wb(rd_address_wb);
    dut->rd_data_wb(rd_data_wb);


    rst = 1;
    stall = 0;
    for(const auto& [sig, data]: std::views::zip(rom, ROM)) {
        sig = data;
    }

    sc_start(SC_ZERO_TIME);
    Verilated::mkdir("logs");
    tfp = new VerilatedFstSc;
    dut->trace(tfp, 99);
    tfp->open("logs/writeback_tb.fst");
    std::signal(SIGABRT, [](int signal) { if(tfp) { tfp->flush(); tfp->close(); }});

    // reset
    sc_start(1, SC_NS);
    rst = 0;
    sc_start(1, SC_NS);
    Predictor{} == dut;
    rst = 1;
    sc_start(1, SC_NS);

    sc_start(5, SC_NS);
    sc_start(5, SC_NS);
    sc_start(5, SC_NS);
    sc_start(5, SC_NS);
    sc_start(5, SC_NS);
    sc_start(5, SC_NS);

    // register file writeback with sll $0, $0, 0 or nops
    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        sc_start(5, SC_NS);
        Predictor {
            .pc_wb = i * 4U,
            .rd_wb = 1,
            .rd_address_wb = i,
            .rd_data_wb = ((i == 0) ? 0 : (((i == 1) ? (-(i + 100)) : (i + 100)))),
        } == dut;
        sc_start(5, SC_NS);
    }

    // register file read with ori rd, rs, 0
    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        sc_start(5, SC_NS);
        Predictor {
            .pc_wb = (i + Constants::REG_COUNT) * 4U,
            .rd_wb = 1,
            .rd_address_wb = i,
            .rd_data_wb = ((i == 0) ? 0 : (((i == 1) ? (-(i + 100)) : (i + 100)))),
        } == dut;
        sc_start(5, SC_NS);
    }

    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        // put register +100 value into forwarder with addi, $0,$1,100
        sc_start(5, SC_NS);
        Predictor {
            .pc_wb = (2U*i + Constants::REG_COUNT*2U) * 4U,
            .rd_wb = 1,
            .rd_address_wb = 0,
            .rd_data_wb = ((i == 0) ? 0 : (((i == 1) ? (-(i + 200)) : (i + 200)))),
        } == dut;
        sc_start(5, SC_NS);
        
        // memory store with sw $0, 4($10)
        sc_start(5, SC_NS);
        Predictor {
            .pc_wb = (2U*i + 1U + Constants::REG_COUNT*2U) * 4U,
            .rd_wb = 0,
            .rd_address_wb = 0,
            .rd_data_wb = 0,
        } == dut;
        sc_start(5, SC_NS);
    }

    // xor zero,zero,zero
    sc_start(5, SC_NS);
    Predictor {
        .pc_wb = (Constants::REG_COUNT + Constants::REG_COUNT*3U) * 4U,
        .rd_wb = 1,
        .rd_address_wb = 0,
        .rd_data_wb = 0,
    } == dut;
    sc_start(5, SC_NS);

    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        sc_start(5, SC_NS);
        Predictor {
            .pc_wb = (2U*i + Constants::REG_COUNT*4U + 1U) * 4U,
            .rd_wb = 1,
            .rd_address_wb = 0,
            .rd_data_wb = ((i == 0) ? 0 : (((i == 1) ? (-(i + 200)) : (i + 200)))),
        } == dut;
        sc_start(5, SC_NS);

        sc_start(5, SC_NS);
        Predictor {
            .pc_wb = (2U*i + 1U + (Constants::REG_COUNT*4U + 1U)) * 4U,
            .rd_wb = 1,
            .rd_address_wb = 0,
            .rd_data_wb = 0,
        } == dut;
        sc_start(5, SC_NS);
    }

    for(const Predictor& i: predictor) {
        sc_start(5, SC_NS);
        i == dut;
        sc_start(5, SC_NS);
    }

    if(tfp) { tfp->flush(); tfp->close(); }
    dut->final();
    return 0;
}