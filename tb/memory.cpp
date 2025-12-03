#include <systemc>
#include <ranges>
#include <vector>
#include <csignal>
#include <verilated.h>
#include <verilated_fst_sc.h>
#include "Vmemory.h"
#include "util.hpp"

using namespace sc_core;
using namespace sc_dt;

struct Predictor {
    sc_bv<32> pc_me { 0 };
    bool load_me { 0 };
    bool alu_mode_me { 0 };
    sc_bv<32> alu_result_me { 0 };
    bool rd_me { 0 };
    sc_bv<5> rd_address_me { 0 };
    sc_bv<32> read_data_me { 0 };

    void operator==(const std::unique_ptr<Vmemory>& dut) const {
        assert(pc_me == dut->pc_me.read());
        assert(load_me == dut->load_me.read());
        assert(alu_mode_me == dut->alu_mode_me.read());
        assert(alu_result_me == dut->alu_result_me.read());
        assert(rd_me == dut->rd_me.read());
        assert(rd_address_me == dut->rd_address_me.read());
        assert(read_data_me == dut->read_data_me.read());
    }
};

const Predictor predictor[] {
    {
        // add      $25,$15,$2
        .pc_me = 640,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115 + 102),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // addu     $25,$15,$2
        .pc_me = 644,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115 + 102),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // sub      $25,$15,$2
        .pc_me = 648,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115 - 102),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // subu     $25,$15,$2
        .pc_me = 652,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115U - 102U),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // and      $25,$15,$2
        .pc_me = 656,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115U & 102U),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // or       $25,$15,$2
        .pc_me = 660,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115U | 102U),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // xor      $25,$15,$2
        .pc_me = 664,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115U ^ 102U),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // nor      $25,$15,$2
        .pc_me = 668,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(~(115U | 102U)),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // sllv     $25,$15,$2
        .pc_me = 672,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = (115U << (102U & 0b1'1111U)),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // srlv     $25,$15,$2
        .pc_me = 676,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = (115U >> (102U & 0b1'1111U)),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // srav     $25,$15,$2
        .pc_me = 680,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = (int32_t(115) >> (102U & 0b1'1111U)),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // slt      $25,$15,$2
        .pc_me = 684,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115 < 102),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // sltu     $25,$15,$2
        .pc_me = 688,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(115U < 102U),
        .rd_me = 1,
        .rd_address_me = 25,
        .read_data_me = 0,
    },
    {
        // sll      $15,$2,0x1e
        .pc_me = 692,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102U << 0x1eU),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // srl      $15,$2,0x1e
        .pc_me = 696,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102U >> 0x1eU),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // sra      $15,$2,0x1e
        .pc_me = 700,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102U >> 0x1eU),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // addi     $15,$2,-32000
        .pc_me = 704,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102 + (-32000)),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // addiu    $15,$2,-32000
        .pc_me = 708,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102 + (-32000)),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // slti     $15,$2,-32000
        .pc_me = 712,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102 < (-32000)),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // sltiu    $15,$2,-32000
        .pc_me = 716,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102 < uint16_t(-32000)),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // andi     $15,$2,0x7d00
        .pc_me = 720,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102 & 0x7d00U),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // ori     $15,$2,0x7d00
        .pc_me = 724,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102 | 0x7d00U),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // xori     $15,$2,0x7d00
        .pc_me = 728,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(102 ^ 0x7d00U),
        .rd_me = 1,
        .rd_address_me = 15,
        .read_data_me = 0,
    },
    {
        // lui      $14,0x7d00
        .pc_me = 732,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(0x7d00U << 16U),
        .rd_me = 1,
        .rd_address_me = 14,
        .read_data_me = 0,
    },

    {
        // lui	$17,0xcece
        .pc_me = 736,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(0xceceU << 16U),
        .rd_me = 1,
        .rd_address_me = 17,
        .read_data_me = 0,
    },
    {
        // ori	$17,$17,0xbaba
        .pc_me = 740,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(0xceceU << 16U) | 0xbabaU,
        .rd_me = 1,
        .rd_address_me = 17,
        .read_data_me = 0,
    },
    {
        // sb	$17,0(zero)
        .pc_me = 744,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // lb	$10,0(zero)
        .pc_me = 748,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = (
            sc_bv<24>((0xbaU & 0x80u) ? 0xffffffu : 0x000000u),
            sc_bv<8>(0xbaU)
        ),
    },
    {
        // lbu	$10,0(zero)
        .pc_me = 752,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbaU,
    },
    {
        // lh	$10,0(zero)
        .pc_me = 756,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbaU,
    },
    {
        // lhu	$10,0(zero)
        .pc_me = 760,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbaU,
    },
    {
        // lw	$10,0(zero)
        .pc_me = 764,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbaU,
    },

    {
        // lui	$17,0xcece
        .pc_me = 768,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(0xceceU << 16U),
        .rd_me = 1,
        .rd_address_me = 17,
        .read_data_me = 0,
    },
    {
        // ori	$17,$17,0xbaba
        .pc_me = 772,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(0xceceU << 16U) | 0xbabaU,
        .rd_me = 1,
        .rd_address_me = 17,
        .read_data_me = 0,
    },
    {
        // sh	$17,0(zero)
        .pc_me = 776,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // lb	$10,0(zero)
        .pc_me = 780,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = (
            sc_bv<24>((0xbaU & 0x80u) ? 0xffffffu : 0x000000u),
            sc_bv<8>(0xbaU)
        ),
    },
    {
        // lbu	$10,0(zero)
        .pc_me = 784,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbaU,
    },
    {
        // lh	$10,0(zero)
        .pc_me = 788,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = (
            sc_bv<16>((0xbabaU & 0x80u) ? 0xffffu : 0x0000u),
            sc_bv<16>(0xbabaU)
        ),
    },
    {
        // lhu	$10,0(zero)
        .pc_me = 792,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbabaU,
    },
    {
        // lw	$10,0(zero)
        .pc_me = 796,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbabaU,
    },

    {
        // lui	$17,0xcece
        .pc_me = 800,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(0xceceU << 16U),
        .rd_me = 1,
        .rd_address_me = 17,
        .read_data_me = 0,
    },
    {
        // ori	$17,$17,0xbaba
        .pc_me = 804,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = uint32_t(0xceceU << 16U) | 0xbabaU,
        .rd_me = 1,
        .rd_address_me = 17,
        .read_data_me = 0,
    },
    {
        // sw	$17,0(zero)
        .pc_me = 808,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // lb	$10,0(zero)
        .pc_me = 812,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = (
            sc_bv<24>((0xbaU & 0x80u) ? 0xffffffu : 0x000000u),
            sc_bv<8>(0xbaU)
        ),
    },
    {
        // lbu	$10,0(zero)
        .pc_me = 816,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbaU,
    },
    {
        // lh	$10,0(zero)
        .pc_me = 820,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = (
            sc_bv<16>((0xbabaU & 0x80u) ? 0xffffu : 0x0000u),
            sc_bv<16>(0xbabaU)
        ),
    },
    {
        // lhu	$10,0(zero)
        .pc_me = 824,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xbabaU,
    },
    {
        // lw	$10,0(zero)
        .pc_me = 828,
        .load_me = 1,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0xcecebabaU,
    },

    {
        // beq	    $14,$10,1 <main+0x10>
        .pc_me = 832,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // bne	    $10,$10,-3 <main+0x8>
        .pc_me = 836,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // blez	$14,1 <main+0x20>
        .pc_me = 840,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // bgtz     $14,-3 <main+0x128>
        .pc_me = 844,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // bltz     $14,5 <main+0x140>
        .pc_me = 848,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // bgez     $11,-7 <main+0x138>
        .pc_me = 852,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // bltzal   $11,5 <main+0x138>
        .pc_me = 856,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 856 + 4 + 4,
        .rd_me = 0,
        .rd_address_me = 31,
        .read_data_me = 0,
    },
    {
        // bgezal   $1,-9 <main+0x130>
        .pc_me = 860,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 860 + 4 + 4,
        .rd_me = 0,
        .rd_address_me = 31,
        .read_data_me = 0,
    },
    {
        // j        880 (880 / 4 == 220)
        .pc_me = 864,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_me = 868,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // jal      896 (896 / 4 = 112)
        .pc_me = 880,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 880 + 4 + 4,
        .rd_me = 1,
        .rd_address_me = 31,
        .read_data_me = 0,
    },
    {
        // ori	$10,$0,912
        .pc_me = 884,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 912,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0,
    },
    {
        // jr       $10
        .pc_me = 896,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // ori	$10,$0,928
        .pc_me = 900,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 928,
        .rd_me = 1,
        .rd_address_me = 10,
        .read_data_me = 0,
    },
    {
        // jalr     $10
        .pc_me = 912,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 912 + 4 + 4,
        .rd_me = 1,
        .rd_address_me = 31,
        .read_data_me = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_me = 916,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_me = 928,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_me = 932,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_me = 936,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_me = 932,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_me = 936,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_me = 932,
        .load_me = 0,
        .alu_mode_me = 0,
        .alu_result_me = 0,
        .rd_me = 0,
        .rd_address_me = 0,
        .read_data_me = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_me = 936,
        .load_me = 0,
        .alu_mode_me = 1,
        .alu_result_me = 0,
        .rd_me = 1,
        .rd_address_me = 0,
        .read_data_me = 0,
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
    sc_signal<bool> nrst;
    const uint8_t ROM[] {
        // nop; sll $0, $0, 0 type instructions for writeback check
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,

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

        0x00,0x00,0x00,0x00, // sll     $0,$0,0
        0xac,0x00,0x00,0x00, // sw	zero,0(zero)
        0x20,0x21,0xff,0x9c, // addi	at,at,-100
        0xac,0x01,0x00,0x04, // sw	at,4(zero)
        0x20,0x42,0x00,0x64, // addi	v0,v0,100
        0xac,0x02,0x00,0x08, // sw	v0,8(zero)
        0x20,0x63,0x00,0x64, // addi	v1,v1,100
        0xac,0x03,0x00,0x0c, // sw	v1,12(zero)
        0x20,0x84,0x00,0x64, // addi	a0,a0,100
        0xac,0x04,0x00,0x10, // sw	a0,16(zero)
        0x20,0xa5,0x00,0x64, // addi	a1,a1,100
        0xac,0x05,0x00,0x14, // sw	a1,20(zero)
        0x20,0xc6,0x00,0x64, // addi	a2,a2,100
        0xac,0x06,0x00,0x18, // sw	a2,24(zero)
        0x20,0xe7,0x00,0x64, // addi	a3,a3,100
        0xac,0x07,0x00,0x1c, // sw	a3,28(zero)
        0x21,0x08,0x00,0x64, // addi	t0,t0,100
        0xac,0x08,0x00,0x20, // sw	t0,32(zero)
        0x21,0x29,0x00,0x64, // addi	t1,t1,100
        0xac,0x09,0x00,0x24, // sw	t1,36(zero)
        0x21,0x4a,0x00,0x64, // addi	t2,t2,100
        0xac,0x0a,0x00,0x28, // sw	t2,40(zero)
        0x21,0x6b,0x00,0x64, // addi	t3,t3,100
        0xac,0x0b,0x00,0x2c, // sw	t3,44(zero)
        0x21,0x8c,0x00,0x64, // addi	t4,t4,100
        0xac,0x0c,0x00,0x30, // sw	t4,48(zero)
        0x21,0xad,0x00,0x64, // addi	t5,t5,100
        0xac,0x0d,0x00,0x34, // sw	t5,52(zero)
        0x21,0xce,0x00,0x64, // addi	t6,t6,100
        0xac,0x0e,0x00,0x38, // sw	t6,56(zero)
        0x21,0xef,0x00,0x64, // addi	t7,t7,100
        0xac,0x0f,0x00,0x3c, // sw	t7,60(zero)
        0x22,0x10,0x00,0x64, // addi	s0,s0,100
        0xac,0x10,0x00,0x40, // sw	s0,64(zero)
        0x22,0x31,0x00,0x64, // addi	s1,s1,100
        0xac,0x11,0x00,0x44, // sw	s1,68(zero)
        0x22,0x52,0x00,0x64, // addi	s2,s2,100
        0xac,0x12,0x00,0x48, // sw	s2,72(zero)
        0x22,0x73,0x00,0x64, // addi	s3,s3,100
        0xac,0x13,0x00,0x4c, // sw	s3,76(zero)
        0x22,0x94,0x00,0x64, // addi	s4,s4,100
        0xac,0x14,0x00,0x50, // sw	s4,80(zero)
        0x22,0xb5,0x00,0x64, // addi	s5,s5,100
        0xac,0x15,0x00,0x54, // sw	s5,84(zero)
        0x22,0xd6,0x00,0x64, // addi	s6,s6,100
        0xac,0x16,0x00,0x58, // sw	s6,88(zero)
        0x22,0xf7,0x00,0x64, // addi	s7,s7,100
        0xac,0x17,0x00,0x5c, // sw	s7,92(zero)
        0x23,0x18,0x00,0x64, // addi	t8,t8,100
        0xac,0x18,0x00,0x60, // sw	t8,96(zero)
        0x23,0x39,0x00,0x64, // addi	t9,t9,100
        0xac,0x19,0x00,0x64, // sw	t9,100(zero)
        0x23,0x5a,0x00,0x64, // addi	k0,k0,100
        0xac,0x1a,0x00,0x68, // sw	k0,104(zero)
        0x23,0x7b,0x00,0x64, // addi	k1,k1,100
        0xac,0x1b,0x00,0x6c, // sw	k1,108(zero)
        0x23,0x9c,0x00,0x64, // addi	gp,gp,100
        0xac,0x1c,0x00,0x70, // sw	gp,112(zero)
        0x23,0xbd,0x00,0x64, // addi	sp,sp,100
        0xac,0x1d,0x00,0x74, // sw	sp,116(zero)
        0x23,0xde,0x00,0x64, // addi	s8,s8,100
        0xac,0x1e,0x00,0x78, // sw	s8,120(zero)
        0x23,0xff,0x00,0x64, // addi	ra,ra,100
        0xac,0x1f,0x00,0x7c, // sw	ra,124(zero)

        0x8c,0x00,0x00,0x00, // lw	zero,0(zero)
        0x8c,0x01,0x00,0x04, // lw	at,4(zero)
        0x8c,0x02,0x00,0x08, // lw	v0,8(zero)
        0x8c,0x03,0x00,0x0c, // lw	v1,12(zero)
        0x8c,0x04,0x00,0x10, // lw	a0,16(zero)
        0x8c,0x05,0x00,0x14, // lw	a1,20(zero)
        0x8c,0x06,0x00,0x18, // lw	a2,24(zero)
        0x8c,0x07,0x00,0x1c, // lw	a3,28(zero)
        0x8c,0x08,0x00,0x20, // lw	t0,32(zero)
        0x8c,0x09,0x00,0x24, // lw	t1,36(zero)
        0x8c,0x0a,0x00,0x28, // lw	t2,40(zero)
        0x8c,0x0b,0x00,0x2c, // lw	t3,44(zero)
        0x8c,0x0c,0x00,0x30, // lw	t4,48(zero)
        0x8c,0x0d,0x00,0x34, // lw	t5,52(zero)
        0x8c,0x0e,0x00,0x38, // lw	t6,56(zero)
        0x8c,0x0f,0x00,0x3c, // lw	t7,60(zero)
        0x8c,0x10,0x00,0x40, // lw	s0,64(zero)
        0x8c,0x11,0x00,0x44, // lw	s1,68(zero)
        0x8c,0x12,0x00,0x48, // lw	s2,72(zero)
        0x8c,0x13,0x00,0x4c, // lw	s3,76(zero)
        0x8c,0x14,0x00,0x50, // lw	s4,80(zero)
        0x8c,0x15,0x00,0x54, // lw	s5,84(zero)
        0x8c,0x16,0x00,0x58, // lw	s6,88(zero)
        0x8c,0x17,0x00,0x5c, // lw	s7,92(zero)
        0x8c,0x18,0x00,0x60, // lw	t8,96(zero)
        0x8c,0x19,0x00,0x64, // lw	t9,100(zero)
        0x8c,0x1a,0x00,0x68, // lw	k0,104(zero)
        0x8c,0x1b,0x00,0x6c, // lw	k1,108(zero)
        0x8c,0x1c,0x00,0x70, // lw	gp,112(zero)
        0x8c,0x1d,0x00,0x74, // lw	sp,116(zero)
        0x8c,0x1e,0x00,0x78, // lw	s8,120(zero)
        0x8c,0x1f,0x00,0x7c, // lw	ra,124(zero)

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

        0x08,0x00,0x00,0xdc, // j	880 <main+0x370>
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x0c,0x00,0x00,0xe0, // jal	896 <main+0x380>
        0x34,0x0a,0x03,0x90, // ori	$10,$0,912
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x01,0x40,0x00,0x08, // jr	$10
        0x34,0x0a,0x03,0xa0, // ori	$10,$0,928
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x01,0x40,0xf8,0x09, // jalr $10
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

    static_assert((sizeof(ROM) > 4) && ((sizeof(ROM) % 4) == 0));
    std::vector<sc_signal<sc_bv<8>>> rom(std::extent_v<std::remove_reference_t<decltype(Vmemory::rom)>>);
    sc_signal<bool> stall;
    sc_signal<bool> rd_wb;
    sc_signal<sc_bv<5>> rd_address_wb;
    sc_signal<sc_bv<32>> rd_data_wb;

    // outputs
    sc_signal<sc_bv<32>> pc_me;
    std::vector<sc_signal<sc_bv<8>>> ram(std::extent_v<std::remove_reference_t<decltype(Vmemory::ram)>>);
    sc_signal<bool> load_me;
    sc_signal<bool> alu_mode_me;
    sc_signal<sc_bv<32>> alu_result_me;
    sc_signal<bool> rd_me;
    sc_signal<sc_bv<5>> rd_address_me;
    sc_signal<sc_bv<32>> read_data_me;
    std::vector<sc_signal<sc_bv<32>>> reg_file(std::extent_v<std::remove_reference_t<decltype(Vmemory::reg_file)>>);

    const std::unique_ptr<Vmemory> dut{new Vmemory{"memory_context"}};

    // inputs
    dut->clk(clk);
    dut->nrst(nrst);
    for(const auto& [port, sig]: std::views::zip(dut->rom, rom)) {
        port(sig);
    }
    dut->stall(stall);
    dut->rd_wb(rd_wb);
    dut->rd_address_wb(rd_address_wb);
    dut->rd_data_wb(rd_data_wb);

    // outputs
    dut->pc_me(pc_me);
    for(const auto& [port, sig]: std::views::zip(dut->ram, ram)) {
        port(sig);
    }
    for(const auto& [port, sig]: std::views::zip(dut->reg_file, reg_file)) {
        port(sig);
    }
    dut->load_me(load_me);
    dut->alu_mode_me(alu_mode_me);
    dut->alu_result_me(alu_result_me);
    dut->rd_me(rd_me);
    dut->rd_address_me(rd_address_me);
    dut->read_data_me(read_data_me);


    nrst = 1;
    stall = 0;
    for(const auto& [data, sig]: std::views::zip(ROM, rom)) {
        sig = data;
    }
    rd_wb = 0;
    rd_address_wb = 0;
    rd_data_wb = 0;

    sc_start(SC_ZERO_TIME);
    Verilated::mkdir("logs");
    tfp = new VerilatedFstSc;
    dut->trace(tfp, 99);
    tfp->open("logs/memory_tb.fst");
    std::signal(SIGABRT, [](int signal) { if(tfp) { tfp->flush(); tfp->close(); }});

    // reset
    sc_start(1, SC_NS);
    nrst = 0;
    sc_start(1, SC_NS);
    Predictor{} == dut;
    nrst = 1;
    sc_start(1, SC_NS);

    // register file writeback with sll $0, $0, 0 or nops
    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        rd_wb = 1;
        rd_address_wb = i + 0;
        rd_data_wb = (i == 1) ? (-(i + 100)) : (i + 100);
        sc_start(5, SC_NS);
        sc_start(5, SC_NS);
    }
    rd_wb = 0;
    rd_address_wb = 0;
    rd_data_wb = 0;

    sc_start(5, SC_NS);
    sc_start(5, SC_NS);
    sc_start(5, SC_NS);
    sc_start(5, SC_NS);
    sc_start(5, SC_NS);
    sc_start(5, SC_NS);

    // register file read with ori rd, rs, 0
    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        sc_start(5, SC_NS);
        Predictor {
            .pc_me = (i + Constants::REG_COUNT) * 4U,
            .load_me = 0,
            .alu_mode_me = 1,
            .alu_result_me = ((i == 0) ? 0 : (((i == 1) ? (-(i + 100)) : (i + 100)))),
            .rd_me = 1,
            .rd_address_me = i,
            .read_data_me = 0,
        } == dut;
        sc_start(5, SC_NS);
    }

    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        // put register +100 value into forwarder with addi, $1,$1,100
        sc_start(5, SC_NS);
        Predictor {
            .pc_me = (2U*i + Constants::REG_COUNT*2U) * 4U,
            .load_me = 0,
            .alu_mode_me = 1,
            .alu_result_me = ((i == 0) ? 0 : (((i == 1) ? (-(i + 200)) : (i + 200)))),
            .rd_me = 1,
            .rd_address_me = i,
            .read_data_me = 0,
        } == dut;
        sc_start(5, SC_NS);
        
        // memory store with sw $1, 4($0)
        sc_start(5, SC_NS);
        Predictor {
            .pc_me = (2U*i + 1U + Constants::REG_COUNT*2U) * 4U,
            .load_me = 0,
            .alu_mode_me = 0,
            .alu_result_me = i * 4,
            .rd_me = 0,
            .rd_address_me = 0,
            .read_data_me = 0,
        } == dut;
        sc_start(5, SC_NS);
    }

    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        // put register +100 value into forwarder with addi, $1,$1,100
        sc_start(5, SC_NS);
        Predictor {
            .pc_me = (i + Constants::REG_COUNT*4U) * 4U,
            .load_me = 1,
            .alu_mode_me = 0,
            .alu_result_me = i * 4,
            .rd_me = 1,
            .rd_address_me = i,
            .read_data_me = ((i == 0) ? 0 : (((i == 1) ? (-(i + 200)) : (i + 200)))),
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