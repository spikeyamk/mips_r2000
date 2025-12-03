#include <memory>
#include <systemc>
#include <ranges>
#include <vector>
#include <csignal>
#include <verilated.h>
#include <verilated_fst_sc.h>
#include "Vdecode.h"
#include "util.hpp"

using namespace sc_core;
using namespace sc_dt;

struct Predictor {
    sc_bv<32> pc_id { 0 };

    bool rs_id { false };
    sc_bv<5> rs_address_id { 0 };
    sc_bv<32> rs_data_id { 0 };
    bool rt_id { false };
    sc_bv<5> rt_address_id { 0 };
    sc_bv<32> rt_data_id { 0 };
    bool rd_id { false };
    sc_bv<5> rd_address_id { 0 };

    bool shamt_id { false };
    sc_bv<5> shamt_value_id { 0 };
    bool imm_id { false };
    sc_bv<16> imm_value_id { 0 };
    bool target_id { false };
    sc_bv<26> target_value_id { 0 };

    bool alu_mode_id { false };
    sc_bv<5> alu_mode_value_id { 0 };
    bool link_id { false };
    bool branch_id { false };
    sc_bv<3> branch_mode_id { 0 };
    bool jump_id { false };

    bool lui_id { false };
    bool load_id { false };
    bool load_sign_extend_id { false };
    sc_bv<2> load_store_data_size_mode_id { 0 };
    bool store_id { false };

    void operator==(const std::unique_ptr<Vdecode>& dut) const {
        assert(pc_id == dut->pc_id.read());
        assert(rs_id == dut->rs_id.read());
        assert(rs_address_id == dut->rs_address_id.read());
        assert(rs_data_id == dut->rs_data_id.read());
        assert(rt_id == dut->rt_id.read());
        assert(rt_address_id == dut->rt_address_id.read());
        assert(rt_data_id == dut->rt_data_id.read());
        assert(rd_id == dut->rd_id.read());
        assert(rd_address_id == dut->rd_address_id.read());
        assert(shamt_id == dut->shamt_id.read());
        assert(shamt_value_id == dut->shamt_value_id.read());
        assert(imm_id == dut->imm_id.read());
        assert(imm_value_id == dut->imm_value_id.read());
        assert(target_id == dut->target_id.read());
        assert(target_value_id == dut->target_value_id.read());
        assert(alu_mode_id == dut->alu_mode_id.read());
        assert(alu_mode_value_id == dut->alu_mode_value_id.read());
        assert(link_id == dut->link_id.read());
        assert(branch_id == dut->branch_id.read());
        assert(branch_mode_id == dut->branch_mode_id.read());
        assert(jump_id == dut->jump_id.read());
        assert(lui_id == dut->lui_id.read());
        assert(load_id == dut->load_id.read());
        assert(load_sign_extend_id == dut->load_sign_extend_id.read());
        assert(load_store_data_size_mode_id == dut->load_store_data_size_mode_id.read());
        assert(store_id == dut->store_id.read());
    }
};

const Predictor predictor[] {
    {
        // instruction = 32'h01e2c820; // add	$25,$15,$2
        .pc_id = 128,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_ADD,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c821; // addu	$25,$15,$2
        .pc_id = 132,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_ADDU,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c822; // sub	$25,$15,$2
        .pc_id = 136,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SUB,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c823; // subu	$25,$15,$2
        .pc_id = 140,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SUBU,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c824; // and	$25,$15,$2
        .pc_id = 144,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_AND,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c825; // or	$25,$15,$2
        .pc_id = 148,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_OR,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c826; // xor	$25,$15,$2
        .pc_id = 152,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_XOR,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c827; // nor	$25,$15,$2
        .pc_id = 156,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_NOR,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h004fc804; // sllv	$25,$15,$2
        .pc_id = 160,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SLL,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h004fc806; // srlv	$25,$15,$2
        .pc_id = 164,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SRL,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h004fc807; // srav	$25,$15,$2
        .pc_id = 168,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SRA,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c82a; // slt	$25,$15,$2
        .pc_id = 172,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SLT,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01e2c82b; // sltu	$25,$15,$2
        .pc_id = 176,
        .rs_id = 1,
        .rs_address_id = 15,
        .rs_data_id = 115,
        .rt_id = 1,
        .rt_address_id = 2,
        .rt_data_id = 102,
        .rd_id = 1,
        .rd_address_id = 25,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SLTU,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h00027f80; // sll	$15,$2,0x1e
        .pc_id = 180,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 1,
        .shamt_value_id = 0x1e,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SLL,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h00027f82; // srl	$15,$2,0x1e
        .pc_id = 184,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 1,
        .shamt_value_id = 0x1e,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SRL,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h00027f83; // sra	$15,$2,0x1e
        .pc_id = 188,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 1,
        .shamt_value_id = 0x1e,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SRA,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h204f8300; // addi	$15,$2,-32000
        .pc_id = 192,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-32000),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_ADD,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h244f8300; // addiu	$15,$2,-32000
        .pc_id = 196,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-32000),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_ADDU,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h284f8300; // slti	$15,$2,-32000
        .pc_id = 200,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-32000),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SLT,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h2c4f8300; // sltiu	$15,$2,-32000
        .pc_id = 204,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-32000),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SLTU,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h304f7d00; // andi	$15,$2,0x7d00
        .pc_id = 208,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = 0x7d00,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_AND,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h344f7d00; // ori	$15,$2,0x7d00
        .pc_id = 212,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = 0x7d00,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_OR,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h384f7d00; // xori	$15,$2,0x7d00
        .pc_id = 216,
        .rs_id = 1,
        .rs_address_id = 2,
        .rs_data_id = 102,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 15,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = 0x7d00,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_XOR,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h3c0e7d00; // lui	$14,0x7d00
        .pc_id = 220,
        .rs_id = 0,
        .rs_address_id = 0,
        .rs_data_id = 0,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 14,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = 0x7d00,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_SLL,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 1,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h81420004; // lb	$2,4($10)
        .pc_id = 224,
        .rs_id = 1,
        .rs_address_id = 10,
        .rs_data_id = 110,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 2,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(4),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 1,
        .load_sign_extend_id = 1,
        .load_store_data_size_mode_id = Decode::LoadStoreDataSizeMode_BYTE,
        .store_id = 0,
    },
    {
        // instruction = 32'h93eefffc; // lbu	$14,-4($31)
        .pc_id = 228,
        .rs_id = 1,
        .rs_address_id = 31,
        .rs_data_id = 131,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 14,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-4),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 1,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = Decode::LoadStoreDataSizeMode_BYTE,
        .store_id = 0,
    },
    {
        // instruction = 32'h87ee0004; // lh	$14,4($31)
        .pc_id = 232,
        .rs_id = 1,
        .rs_address_id = 31,
        .rs_data_id = 131,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 14,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(4),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 1,
        .load_sign_extend_id = 1,
        .load_store_data_size_mode_id = Decode::LoadStoreDataSizeMode_HALF_WORD,
        .store_id = 0,
    },
    {
        // instruction = 32'h97eefffc; // lhu	$14,-4($31)
        .pc_id = 236,
        .rs_id = 1,
        .rs_address_id = 31,
        .rs_data_id = 131,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 14,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-4),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 1,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = Decode::LoadStoreDataSizeMode_HALF_WORD,
        .store_id = 0,
    },
    {
        // instruction = 32'h8fee0008; // lw	$14,8($31)
        .pc_id = 240,
        .rs_id = 1,
        .rs_address_id = 31,
        .rs_data_id = 131,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 14,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(8),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 1,
        .load_sign_extend_id = 1,
        .load_store_data_size_mode_id = Decode::LoadStoreDataSizeMode_WORD,
        .store_id = 0,
    },
    {
        // instruction = 32'ha3ee0008; // sb	$14,8($31)
        .pc_id = 244,
        .rs_id = 1,
        .rs_address_id = 31,
        .rs_data_id = 131,
        .rt_id = 1,
        .rt_address_id = 14,
        .rt_data_id = 114,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(8),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = Decode::LoadStoreDataSizeMode_BYTE,
        .store_id = 1,
    },
    {
        // instruction = 32'ha7eefff8; // sh	$14,-8($31)
        .pc_id = 248,
        .rs_id = 1,
        .rs_address_id = 31,
        .rs_data_id = 131,
        .rt_id = 1,
        .rt_address_id = 14,
        .rt_data_id = 114,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-8),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = Decode::LoadStoreDataSizeMode_HALF_WORD,
        .store_id = 1,
    },
    {
        // instruction = 32'hafee0008; // sw	$14,8($31)
        .pc_id = 252,
        .rs_id = 1,
        .rs_address_id = 31,
        .rs_data_id = 131,
        .rt_id = 1,
        .rt_address_id = 14,
        .rt_data_id = 114,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(8),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = Decode::LoadStoreDataSizeMode_WORD,
        .store_id = 1,
    },
    {
        // instruction = 32'h11cafffd; // beq	$14,$10,-3 <main+0x110>
        .pc_id = 256,
        .rs_id = 1,
        .rs_address_id = 14,
        .rs_data_id = 114,
        .rt_id = 1,
        .rt_address_id = 10,
        .rt_data_id = 110,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-3),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 1,
        .branch_mode_id = Decode::BranchMode_BEQ,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h15cafffd; // bne	$14,$10,-3 <main+0x118>
        .pc_id = 260,
        .rs_id = 1,
        .rs_address_id = 14,
        .rs_data_id = 114,
        .rt_id = 1,
        .rt_address_id = 10,
        .rt_data_id = 110,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-3),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 1,
        .branch_mode_id = Decode::BranchMode_BNE,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h19c0fffd; // blez	$14,-3 <main+0x120>
        .pc_id = 264,
        .rs_id = 1,
        .rs_address_id = 14,
        .rs_data_id = 114,
        .rt_id = 1,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-3),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 1,
        .branch_mode_id = Decode::BranchMode_BLEZ,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h1dc0fffd; // bgtz	$14,-3 <main+0x128>
        .pc_id = 268,
        .rs_id = 1,
        .rs_address_id = 14,
        .rs_data_id = 114,
        .rt_id = 1,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-3),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 1,
        .branch_mode_id = Decode::BranchMode_BGTZ,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h05c00001; // bltz	$14,1 <main+0x140>
        .pc_id = 272,
        .rs_id = 1,
        .rs_address_id = 14,
        .rs_data_id = 114,
        .rt_id = 1,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(1),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 1,
        .branch_mode_id = Decode::BranchMode_BLTZ,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h0561fffd; // bgez	$11,-3 <main+0x138>
        .pc_id = 276,
        .rs_id = 1,
        .rs_address_id = 11,
        .rs_data_id = 111,
        .rt_id = 1,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-3),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 1,
        .branch_mode_id = Decode::BranchMode_BGEZ,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h0570fffb; // bltzal	$11,-5 <main+0x138>
        .pc_id = 280,
        .rs_id = 1,
        .rs_address_id = 11,
        .rs_data_id = 111,
        .rt_id = 1,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 31,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-5),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_ADDU,
        .link_id = 1,
        .branch_id = 1,
        .branch_mode_id = Decode::BranchMode_BLTZ,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h0571fff7; // bgezal	$11,-9 <main+0x130>
        .pc_id = 284,
        .rs_id = 1,
        .rs_address_id = 11,
        .rs_data_id = 111,
        .rt_id = 1,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 31,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 1,
        .imm_value_id = int16_t(-9),
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_ADDU,
        .link_id = 1,
        .branch_id = 1,
        .branch_mode_id = Decode::BranchMode_BGEZ,
        .jump_id = 0,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h0800000b; // j	44 (44 / 4 == 11)
        .pc_id = 288,
        .rs_id = 0,
        .rs_address_id = 0,
        .rs_data_id = 0,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 1,
        .target_value_id = 0xb,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 1,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h0c000016; // jal	88 (88 / 4 = 22)
        .pc_id = 292,
        .rs_id = 0,
        .rs_address_id = 0,
        .rs_data_id = 0,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 31,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 1,
        .target_value_id = 0x16,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_ADDU,
        .link_id = 1,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 1,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h01400008; // jr	$10
        .pc_id = 296,
        .rs_id = 1,
        .rs_address_id = 10,
        .rs_data_id = 110,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 0,
        .rd_address_id = 0,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 0,
        .alu_mode_value_id = 0,
        .link_id = 0,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 1,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
    },
    {
        // instruction = 32'h0140f809; // jalr	$10
        .pc_id = 300,
        .rs_id = 1,
        .rs_address_id = 10,
        .rs_data_id = 110,
        .rt_id = 0,
        .rt_address_id = 0,
        .rt_data_id = 0,
        .rd_id = 1,
        .rd_address_id = 31,
        .shamt_id = 0,
        .shamt_value_id = 0,
        .imm_id = 0,
        .imm_value_id = 0,
        .target_id = 0,
        .target_value_id = 0,
        .alu_mode_id = 1,
        .alu_mode_value_id = Decode::ALUMode_ADDU,
        .link_id = 1,
        .branch_id = 0,
        .branch_mode_id = 0,
        .jump_id = 1,
        .lui_id = 0,
        .load_id = 0,
        .load_sign_extend_id = 0,
        .load_store_data_size_mode_id = 0,
        .store_id = 0,
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
        // asm("add $1,  $0,  $1"); type instructions for writeback check
        0x00,0x01,0x08,0x20,
        0x00,0x22,0x10,0x20,
        0x00,0x43,0x18,0x20,
        0x00,0x64,0x20,0x20,
        0x00,0x85,0x28,0x20,
        0x00,0xa6,0x30,0x20,
        0x00,0xc7,0x38,0x20,
        0x00,0xe8,0x40,0x20,
        0x01,0x09,0x48,0x20,
        0x01,0x2a,0x50,0x20,
        0x01,0x4b,0x58,0x20,
        0x01,0x6c,0x60,0x20,
        0x01,0x8d,0x68,0x20,
        0x01,0xae,0x70,0x20,
        0x01,0xcf,0x78,0x20,
        0x01,0xf0,0x80,0x20,
        0x02,0x11,0x88,0x20,
        0x02,0x32,0x90,0x20,
        0x02,0x53,0x98,0x20,
        0x02,0x74,0xa0,0x20,
        0x02,0x95,0xa8,0x20,
        0x02,0xb6,0xb0,0x20,
        0x02,0xd7,0xb8,0x20,
        0x02,0xf8,0xc0,0x20,
        0x03,0x19,0xc8,0x20,
        0x03,0x3a,0xd0,0x20,
        0x03,0x5b,0xd8,0x20,
        0x03,0x7c,0xe0,0x20,
        0x03,0x9d,0xe8,0x20,
        0x03,0xbe,0xf0,0x20,
        0x03,0xdf,0xf8,0x20,
        0x03,0xe0,0x00,0x20,

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
        0xa3,0xee,0x00,0x08, // sb       $14,8($31)
        0xa7,0xee,0xff,0xf8, // sh       $14,-8($31)
        0xaf,0xee,0x00,0x08, // sw       $14,8($31)
        0x11,0xca,0xff,0xfd, // beq      $14,$10,-3 <main+0x110>
        0x15,0xca,0xff,0xfd, // bne      $14,$10,-3 <main+0x118>
        0x19,0xc0,0xff,0xfd, // blez     $14,-3 <main+0x120>
        0x1d,0xc0,0xff,0xfd, // bgtz     $14,-3 <main+0x128>
        0x05,0xc0,0x00,0x01, // bltz     $14,1 <main+0x140>
        0x05,0x61,0xff,0xfd, // bgez     $11,-3 <main+0x138>
        0x05,0x70,0xff,0xfb, // bltzal   $11,-5 <main+0x138>
        0x05,0x71,0xff,0xf7, // bgezal   $11,-9 <main+0x130>
        0x08,0x00,0x00,0x0b, // j        44 (44 / 4 == 11)
        0x0c,0x00,0x00,0x16, // jal      88 (88 / 4 = 22)
        0x01,0x40,0x00,0x08, // jr       $10
        0x01,0x40,0xf8,0x09, // jalr     $10
    };

    static_assert((sizeof(ROM) > 4) && ((sizeof(ROM) % 4) == 0));
    std::vector<sc_signal<sc_bv<8>>> rom(std::extent_v<std::remove_reference_t<decltype(Vdecode::rom)>>);
    sc_signal<bool> stall;
    sc_signal<bool> branch_taken_ex;
    sc_signal<sc_bv<32>> branch_target_ex;
    sc_signal<bool> rd_wb;
    sc_signal<sc_bv<5>> rd_address_wb;
    sc_signal<sc_bv<32>> rd_data_wb;

    // outputs
    sc_signal<bool> rs_id;
    sc_signal<bool> rt_id;
    sc_signal<bool> rd_id;
    sc_signal<bool> shamt_id;
    sc_signal<bool> imm_id;
    sc_signal<bool> target_id;
    sc_signal<bool> alu_mode_id;
    sc_signal<bool> link_id;
    sc_signal<bool> branch_id;
    sc_signal<bool> jump_id;
    sc_signal<bool> lui_id;
    sc_signal<bool> load_id;
    sc_signal<bool> load_sign_extend_id;
    sc_signal<bool> store_id;
    sc_signal<sc_bv<32>> pc_id;
    sc_signal<sc_bv<5>> rs_address_id;
    sc_signal<sc_bv<32>> rs_data_id;
    sc_signal<sc_bv<5>> rt_address_id;
    sc_signal<sc_bv<32>> rt_data_id;
    sc_signal<sc_bv<5>> rd_address_id;
    sc_signal<sc_bv<5>> shamt_value_id;
    sc_signal<sc_bv<16>> imm_value_id;
    sc_signal<sc_bv<26>> target_value_id;
    sc_signal<sc_bv<5>> alu_mode_value_id;
    sc_signal<sc_bv<3>> branch_mode_id;
    sc_signal<sc_bv<2>> load_store_data_size_mode_id;
    std::vector<sc_signal<sc_bv<32>>> reg_file(std::extent_v<std::remove_reference_t<decltype(Vdecode::reg_file)>>);

    const std::unique_ptr<Vdecode> dut{new Vdecode{"decode_context"}};

    // inputs
    dut->clk(clk);
    dut->nrst(nrst);
    for(const auto& [port, sig]: std::views::zip(dut->rom, rom)) {
        port(sig);
    }
    dut->stall(stall);
    dut->branch_taken_ex(branch_taken_ex);
    dut->branch_target_ex(branch_target_ex);
    dut->rd_wb(rd_wb);
    dut->rd_address_wb(rd_address_wb);
    dut->rd_data_wb(rd_data_wb);

    // outputs
    dut->rs_id(rs_id);
    dut->rt_id(rt_id);
    dut->rd_id(rd_id);
    dut->shamt_id(shamt_id);
    dut->imm_id(imm_id);
    dut->target_id(target_id);
    dut->alu_mode_id(alu_mode_id);
    dut->link_id(link_id);
    dut->branch_id(branch_id);
    dut->jump_id(jump_id);
    dut->lui_id(lui_id);
    dut->load_id(load_id);
    dut->load_sign_extend_id(load_sign_extend_id);
    dut->store_id(store_id);
    dut->pc_id(pc_id);
    dut->rs_address_id(rs_address_id);
    dut->rs_data_id(rs_data_id);
    dut->rt_address_id(rt_address_id);
    dut->rt_data_id(rt_data_id);
    dut->rd_address_id(rd_address_id);
    dut->shamt_value_id(shamt_value_id);
    dut->imm_value_id(imm_value_id);
    dut->target_value_id(target_value_id);
    dut->alu_mode_value_id(alu_mode_value_id);
    dut->branch_mode_id(branch_mode_id);
    dut->load_store_data_size_mode_id(load_store_data_size_mode_id);
    for(const auto& [port, sig]: std::views::zip(dut->reg_file, reg_file)) {
        port(sig);
    }

    nrst = 1;
    stall = 0;
    branch_taken_ex = 0;
    branch_target_ex = 0;
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
    tfp->open("logs/decode_tb.fst");
    std::signal(SIGABRT, [](int signal) { if(tfp) { tfp->flush(); tfp->close(); }});

    // reset
    sc_start(1, SC_NS);
    nrst = 0;
    sc_start(1, SC_NS);
    Predictor{} == dut;
    nrst = 1;
    sc_start(1, SC_NS);

    // read check empty register file
    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        if(i == 0) {
            // load finrst instruction into fetch stage
            sc_start(5, SC_NS);
            sc_start(5, SC_NS);
        }

        sc_start(5, SC_NS);
        assert(dut->rs_id.read() == 1);
        assert(dut->rs_address_id.read() == i);
        assert(dut->rs_data_id.read() == 0);

        assert(dut->rd_id.read() == 1);
        assert(dut->rd_address_id.read() == i + 1);

        assert(dut->rt_id.read() == 1);
        assert(dut->rt_address_id.read() == i + 1);
        assert(dut->rt_data_id.read() == 0);

        sc_start(5, SC_NS);
    }

    // reset
    nrst = 1;
    sc_start(1, SC_NS);
    nrst = 0;
    sc_start(1, SC_NS);
    Predictor{} == dut;
    nrst = 1;
    sc_start(8, SC_NS);

    // read check register file writeback
    for(const uint32_t i: std::views::iota(0U, Constants::REG_COUNT)) {
        rd_wb = 1;
        rd_address_wb = i + 1;
        rd_data_wb = (i + 1) + 100;
        if(i == 0) {
            // load finrst instruction into fetch stage
            sc_start(5, SC_NS);
            sc_start(5, SC_NS);
        }

        sc_start(5, SC_NS);
        assert(dut->rs_id.read() == 1);
        assert(dut->rs_address_id.read() == i);

        assert(dut->rd_id.read() == 1);
        assert(dut->rd_address_id.read() == i + 1);

        assert(dut->rt_id.read() == 1);
        assert(dut->rt_address_id.read() == i + 1);

        if(i == 0) {
            assert(dut->rs_data_id.read() == 0);
            assert(dut->rt_data_id.read() == (i + 1) + 100);
        } else {
            assert(dut->rs_data_id.read() == (i) + 100);
            assert(dut->rt_data_id.read() == (i == 31 ? 0 : (i + 1) + 100));
        }

        sc_start(5, SC_NS);
    }
    rd_wb = 0;

    for(const Predictor& i: predictor) {
        sc_start(5, SC_NS);
        i == dut;
        sc_start(5, SC_NS);
    }

    if(tfp) { tfp->flush(); tfp->close(); }
    dut->final();
    return 0;
}