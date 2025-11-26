#include <memory>
#include <systemc>
#include <ranges>
#include <vector>
#include <csignal>
#include <verilated.h>
#include <verilated_fst_sc.h>
#include "Vexecute.h"
#include "util.hpp"

using namespace sc_core;
using namespace sc_dt;

struct Predictor {
    sc_bv<32> pc_executed { 0 };
    bool rd_executed { 0 };
    sc_bv<5> rd_address_executed { 0 };
    bool alu_mode_executed { 0 };
    sc_bv<32> alu_result_executed { 0 };
    sc_bv<32> rt_data_excuted { 0 };
    bool load_executed { 0 };
    bool load_sign_extend_executed { 0 };
    sc_bv<2> load_store_data_size_mode_executed { 0 };
    bool store_executed { 0 };

    void operator==(const std::unique_ptr<Vexecute>& dut) const {
        assert(pc_executed == dut->pc_executed.read());
        assert(rd_executed == dut->rd_executed.read());
        assert(rd_address_executed == dut->rd_address_executed.read());
        assert(alu_mode_executed == dut->alu_mode_executed.read());
        assert(alu_result_executed == dut->alu_result_executed.read());
        assert(rt_data_excuted == dut->rt_data_excuted.read());
        assert(load_executed == dut->load_executed.read());
        assert(load_sign_extend_executed == dut->load_sign_extend_executed.read());
        assert(load_store_data_size_mode_executed == dut->load_store_data_size_mode_executed.read());
        assert(store_executed == dut->store_executed.read());
    }
};

const Predictor predictor[] {
    {
        // add      $25,$15,$2
        .pc_executed = 256,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115 + 102),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // addu     $25,$15,$2
        .pc_executed = 260,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115 + 102),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // sub      $25,$15,$2
        .pc_executed = 264,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115 - 102),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // subu     $25,$15,$2
        .pc_executed = 268,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115 - 102),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // and      $25,$15,$2
        .pc_executed = 272,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115 & 102),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // or       $25,$15,$2
        .pc_executed = 276,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115 | 102),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // xor      $25,$15,$2
        .pc_executed = 280,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115 ^ 102),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // nor      $25,$15,$2
        .pc_executed = 284,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = ~(115 | 102),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // sllv     $25,$15,$2
        .pc_executed = 288,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115U << (102U & 0b1'1111U)),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // srlv     $25,$15,$2
        .pc_executed = 292,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (115U >> (102U & 0b1'1111U)),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // srav     $25,$15,$2
        .pc_executed = 296,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = (int32_t(115) >> (102U & 0b1'1111U)),
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // slt      $25,$15,$2
        .pc_executed = 300,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = 0,
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // sltu     $25,$15,$2
        .pc_executed = 304,
        .rd_executed = 1,
        .rd_address_executed = 25,
        .alu_mode_executed = 1,
        .alu_result_executed = 0,
        .rt_data_excuted = 102,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // sll      $15,$2,0x1e
        .pc_executed = 308,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (102 << 0x1e),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // srl      $15,$2,0x1e
        .pc_executed = 312,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (102 >> 0x1e),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // sra      $15,$2,0x1e
        .pc_executed = 316,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (static_cast<int32_t>(102) >> (0x1e)),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // addi     $15,$2,-32000
        .pc_executed = 320,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (102 + (-32000)),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // addiu    $15,$2,-32000
        .pc_executed = 324,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (102 + (-32000)),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // slti     $15,$2,-32000
        .pc_executed = 328,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (102 < -32000),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // sltiu    $15,$2,-32000
        .pc_executed = 332,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (uint16_t(102) < uint16_t(-32000)),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // andi     $15,$2,0x7d00
        .pc_executed = 336,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (102 & 0x7d00),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // ori     $15,$2,0x7d00
        .pc_executed = 340,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (102 | 0x7d00),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // xori     $15,$2,0x7d00
        .pc_executed = 344,
        .rd_executed = 1,
        .rd_address_executed = 15,
        .alu_mode_executed = 1,
        .alu_result_executed = (102 ^ 0x7d00),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // lui      $14,0x7d00
        .pc_executed = 348,
        .rd_executed = 1,
        .rd_address_executed = 14,
        .alu_mode_executed = 1,
        .alu_result_executed = (0x7d00 << 16),
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // lb       $2,4($10)
        .pc_executed = 352,
        .rd_executed = 1,
        .rd_address_executed = 2,
        .alu_mode_executed = 0,
        .alu_result_executed = 110 + 4,
        .rt_data_excuted = 0,
        .load_executed = 1,
        .load_sign_extend_executed = 1,
        .load_store_data_size_mode_executed = Decode::LoadStoreDataSizeMode_BYTE,
        .store_executed = 0,
    },
    {
        // lbu      $14,-4($31)
        .pc_executed = 356,
        .rd_executed = 1,
        .rd_address_executed = 14,
        .alu_mode_executed = 0,
        .alu_result_executed = 131 + (-4),
        .rt_data_excuted = 0,
        .load_executed = 1,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = Decode::LoadStoreDataSizeMode_BYTE,
        .store_executed = 0,
    },
    {
        // lh       $14,4($31)
        .pc_executed = 360,
        .rd_executed = 1,
        .rd_address_executed = 14,
        .alu_mode_executed = 0,
        .alu_result_executed = 131 + (4),
        .rt_data_excuted = 0,
        .load_executed = 1,
        .load_sign_extend_executed = 1,
        .load_store_data_size_mode_executed = Decode::LoadStoreDataSizeMode_HALF_WORD,
        .store_executed = 0,
    },
    {
        // lhu      $14,-4($31)
        .pc_executed = 364,
        .rd_executed = 1,
        .rd_address_executed = 14,
        .alu_mode_executed = 0,
        .alu_result_executed = 131 + (-4),
        .rt_data_excuted = 0,
        .load_executed = 1,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = Decode::LoadStoreDataSizeMode_HALF_WORD,
        .store_executed = 0,
    },
    {
        // lw       $14,8($31)
        .pc_executed = 368,
        .rd_executed = 1,
        .rd_address_executed = 14,
        .alu_mode_executed = 0,
        .alu_result_executed = 131 + (8),
        .rt_data_excuted = 0,
        .load_executed = 1,
        .load_sign_extend_executed = 1,
        .load_store_data_size_mode_executed = Decode::LoadStoreDataSizeMode_WORD,
        .store_executed = 0,
    },
    {
        // sb       $17,8($31)
        .pc_executed = 372,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 131 + (8),
        .rt_data_excuted = 117,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = Decode::LoadStoreDataSizeMode_BYTE,
        .store_executed = 1,
    },
    {
        // sh       $18,-8($31)
        .pc_executed = 376,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 131 + (-8),
        .rt_data_excuted = 118,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = Decode::LoadStoreDataSizeMode_HALF_WORD,
        .store_executed = 1,
    },
    {
        // sw       $19,8($31)
        .pc_executed = 380,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 131 + (8),
        .rt_data_excuted = 119,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = Decode::LoadStoreDataSizeMode_WORD,
        .store_executed = 1,
    },
    {
        // beq	    $14,$10,1 <main+0x10>
        .pc_executed = 384,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 110,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // bne	    $10,$10,-3 <main+0x8>
        .pc_executed = 388,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 110,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // blez	$14,1 <main+0x20>
        .pc_executed = 392,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // bgtz     $14,-3 <main+0x128>
        .pc_executed = 396,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // bltz     $14,5 <main+0x140>
        .pc_executed = 400,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // bgez     $11,-7 <main+0x138>
        .pc_executed = 404,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // bltzal   $11,5 <main+0x138>
        .pc_executed = 408,
        .rd_executed = 0,
        .rd_address_executed = 31,
        .alu_mode_executed = 1,
        .alu_result_executed = 408 + 4 + 4,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // bgezal   $1,-9 <main+0x130>
        .pc_executed = 412,
        .rd_executed = 0,
        .rd_address_executed = 31,
        .alu_mode_executed = 1,
        .alu_result_executed = 412 + 4 + 4,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // j        432 (432 / 4 == 108)
        .pc_executed = 416,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_executed = 420,
        .rd_executed = 1,
        .rd_address_executed = 0,
        .alu_mode_executed = 1,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // jal      448 (448 / 4 = 112)
        .pc_executed = 432,
        .rd_executed = 1,
        .rd_address_executed = 31,
        .alu_mode_executed = 1,
        .alu_result_executed = 432 + 4 + 4, //??
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // ori	     $10,$0,464
        .pc_executed = 436,
        .rd_executed = 1,
        .rd_address_executed = 10,
        .alu_mode_executed = 1,
        .alu_result_executed = 464,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // jr       $10
        .pc_executed = 448,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // ori	     $10,$0,480
        .pc_executed = 452,
        .rd_executed = 1,
        .rd_address_executed = 10,
        .alu_mode_executed = 1,
        .alu_result_executed = 480,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // jalr     $10
        .pc_executed = 464,
        .rd_executed = 1,
        .rd_address_executed = 31,
        .alu_mode_executed = 1,
        .alu_result_executed = 464 + 4 + 4,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_executed = 468,
        .rd_executed = 1,
        .rd_address_executed = 0,
        .alu_mode_executed = 1,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_executed = 480,
        .rd_executed = 1,
        .rd_address_executed = 0,
        .alu_mode_executed = 1,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_executed = 484,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_executed = 488,
        .rd_executed = 1,
        .rd_address_executed = 0,
        .alu_mode_executed = 1,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_executed = 484,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_executed = 488,
        .rd_executed = 1,
        .rd_address_executed = 0,
        .alu_mode_executed = 1,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    // ------------------//------------------ //
    {
        // beq      $0,$0,-1
        .pc_executed = 484,
        .rd_executed = 0,
        .rd_address_executed = 0,
        .alu_mode_executed = 0,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
    },
    {
        // nop -> sll      $0,$0,0x00
        .pc_executed = 488,
        .rd_executed = 1,
        .rd_address_executed = 0,
        .alu_mode_executed = 1,
        .alu_result_executed = 0,
        .rt_data_excuted = 0,
        .load_executed = 0,
        .load_sign_extend_executed = 0,
        .load_store_data_size_mode_executed = 0,
        .store_executed = 0,
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

        // asm("sw $0,  10($1)"); type instructions for writeback check
        0xac,0x20,0x00,0x0a,
        0xac,0x41,0x00,0x0a,
        0xac,0x62,0x00,0x0a,
        0xac,0x83,0x00,0x0a,
        0xac,0xa4,0x00,0x0a,
        0xac,0xc5,0x00,0x0a,
        0xac,0xe6,0x00,0x0a,
        0xad,0x07,0x00,0x0a,
        0xad,0x28,0x00,0x0a,
        0xad,0x49,0x00,0x0a,
        0xad,0x6a,0x00,0x0a,
        0xad,0x8b,0x00,0x0a,
        0xad,0xac,0x00,0x0a,
        0xad,0xcd,0x00,0x0a,
        0xad,0xee,0x00,0x0a,
        0xae,0x0f,0x00,0x0a,
        0xae,0x30,0x00,0x0a,
        0xae,0x51,0x00,0x0a,
        0xae,0x72,0x00,0x0a,
        0xae,0x93,0x00,0x0a,
        0xae,0xb4,0x00,0x0a,
        0xae,0xd5,0x00,0x0a,
        0xae,0xf6,0x00,0x0a,
        0xaf,0x17,0x00,0x0a,
        0xaf,0x38,0x00,0x0a,
        0xaf,0x59,0x00,0x0a,
        0xaf,0x7a,0x00,0x0a,
        0xaf,0x9b,0x00,0x0a,
        0xaf,0xbc,0x00,0x0a,
        0xaf,0xdd,0x00,0x0a,
        0xaf,0xfe,0x00,0x0a,
        0xac,0x1f,0x00,0x0a,
    
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
        0x81,0x42,0x00,0x04, // lb       $2,4($10)
        0x93,0xee,0xff,0xfc, // lbu      $14,-4($31)
        0x87,0xee,0x00,0x04, // lh       $14,4($31)
        0x97,0xee,0xff,0xfc, // lhu      $14,-4($31)
        0x8f,0xee,0x00,0x08, // lw       $14,8($31)
        0xa3,0xf1,0x00,0x08, // sb       $17,8(ra)
        0xa7,0xf2,0xff,0xf8, // sh       $18,-8(ra)
        0xaf,0xf3,0x00,0x08, // sw       $19,8(ra)

        0x11,0xca,0x00,0x01, // beq	    $14,$10,1 <main+0x10>
        0x15,0x4a,0xff,0xfd, // bne	    $10,$10,-3 <main+0x8>
        0x19,0xc0,0x00,0x01, // blez	$14,1 <main+0x20>
        0x1c,0x20,0xff,0xfd, // bgtz	$1,-3 <main+0x18>
        0x05,0xc0,0x00,0x05, // bltz	$14,5 <main+0x40>
        0x04,0x21,0xff,0xf9, // bgez	$1,-7 <main+0x18>
        0x05,0x70,0x00,0x05, // bltzal	$11,5 <main+0x50>
        0x04,0x31,0xff,0xf9, // bgezal	$1,-9 <main+0x28>

        0x08,0x00,0x00,0x6c, // j        432 (432 / 4 == 108)
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x0c,0x00,0x00,0x70, // jal      448 (448 / 4 = 112)
        0x34,0x0a,0x01,0xd0, // ori	     $10,$0,464
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x01,0x40,0x00,0x08, // jr       $10
        0x34,0x0a,0x01,0xe0, // ori	     $10,$0,480
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x01,0x40,0xf8,0x09, // jalr     $10
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop
        0x00,0x00,0x00,0x00, // nop

        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
        0x10,0x00,0xff,0xff, // beq	$0,$0,-1
        0x00,0x00,0x00,0x00, // nop; sll $0, $0, 0 to verify branched variables from previous cycle
    };

    static_assert((sizeof(ROM) > 4) && ((sizeof(ROM) % 4) == 0));
    std::vector<sc_signal<sc_bv<8>>> rom(std::extent_v<std::remove_reference_t<decltype(Vexecute::rom)>>);
    sc_signal<bool> stall;
    sc_signal<bool> rd_wb;
    sc_signal<sc_bv<5>> rd_address_wb;
    sc_signal<sc_bv<32>> rd_data_wb;

    // outputs
    sc_signal<bool> rd_executed;
    sc_signal<bool> alu_mode_executed;
    sc_signal<bool> load_executed;
    sc_signal<bool> load_sign_extend_executed;
    sc_signal<bool> store_executed;

    sc_signal<sc_bv<32>> pc_executed;
    sc_signal<sc_bv<5>> rd_address_executed;
    sc_signal<sc_bv<32>> alu_result_executed;
    sc_signal<sc_bv<32>> rt_data_excuted;
    sc_signal<sc_bv<2>> load_store_data_size_mode_executed;
    std::vector<sc_signal<sc_bv<32>>> reg_file(std::extent_v<std::remove_reference_t<decltype(Vexecute::reg_file)>>);

    const std::unique_ptr<Vexecute> dut{new Vexecute{"execute_context"}};

    // inputs
    dut->clk(clk);
    dut->rst(rst);
    for(const auto& [port, sig]: std::views::zip(dut->rom, rom)) {
        port(sig);
    }
    dut->stall(stall);
    dut->rd_wb(rd_wb);
    dut->rd_address_wb(rd_address_wb);
    dut->rd_data_wb(rd_data_wb);

    // outputs
    dut->rd_executed(rd_executed);
    dut->alu_mode_executed(alu_mode_executed);
    dut->load_executed(load_executed);
    dut->load_sign_extend_executed(load_sign_extend_executed);
    dut->store_executed(store_executed);

    dut->pc_executed(pc_executed);
    dut->rd_address_executed(rd_address_executed);
    dut->alu_result_executed(alu_result_executed);
    dut->rt_data_excuted(rt_data_excuted);
    dut->load_store_data_size_mode_executed(load_store_data_size_mode_executed);
    for(const auto& [port, sig]: std::views::zip(dut->reg_file, reg_file)) {
        port(sig);
    }


    rst = 1;
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
    tfp->open("logs/execute_tb.fst");
    std::signal(SIGABRT, [](int signal) { if(tfp) { tfp->flush(); tfp->close(); }});

    // reset
    sc_start(1, SC_NS);
    rst = 0;
    sc_start(1, SC_NS);
    Predictor{} == dut;
    rst = 1;
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

    // register file read with sw rt, imm(rs)
    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        sc_start(5, SC_NS);
        assert(dut->store_executed.read() == 1);
        if(i == 0) {
            assert(dut->rt_data_excuted.read() == 0);
        } else {
            assert(dut->rt_data_excuted.read() == ((i == 1) ? (-(i + 100)) : (i + 100)));
        }
        
        if(i == Constants::REG_COUNT - 1) {
            assert(dut->alu_result_executed.read() == 0 + 10);
        } else {
            assert(dut->alu_result_executed.read() == ((i + 1 == 1) ? (-(i + 1 + 100) + 10) : ((i + 1 + 100) + 10)));
        }
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