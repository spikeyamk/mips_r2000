#include <memory>
#include <systemc>
#include <ranges>
#include <csignal>
#include <vector>
#include <print>
#include <string_view>
#include <bitset>
#include <verilated.h>
#include <verilated_fst_sc.h>
#include "Vbubble_sort_demo.h"
#include "util.hpp"

using namespace sc_core;
using namespace sc_dt;

const std::array<sc_bv<7>, 16> SEG_ROM {
    0b1000000,
    0b1111001,
    0b0100100,
    0b0110000,
    0b0011001,
    0b0010010,
    0b0000010,
    0b1111000,
    0b0000000,
    0b0011000,
    0b0001000,
    0b0000011,
    0b1000110,
    0b0100001,
    0b0000110,
    0b0001110,
};

struct Predictor {
    sc_bv<3> index { 0 };
    sc_bv<4> number { 0 };

    void operator==(const std::unique_ptr<Vbubble_sort_demo>& dut) const {
        std::bitset<8> an { 0b1111'1111 };
        an[index.to_uint()] = 0;
        assert(an.to_ulong() == dut->an.read()); 
        assert(1 == dut->dp.read());
        assert(SEG_ROM[number.to_uint()] == dut->seg.read());
    }

    static auto generate(const std::array<sc_bv<4>, 8>& numbers) {
        return std::views::transform(std::views::enumerate(std::views::reverse(numbers)), [](const auto& e) {
            return Predictor {
                .index = std::get<0>(e),
                .number = std::get<1>(e),
            };
        });
    }
};

std::ostream& operator<<(std::ostream& os, const Predictor& obj) {
    return os;
}

VerilatedFstSc* tfp = nullptr;

void print_pc(const std::unique_ptr<Vbubble_sort_demo>& dut) {
    //std::printf("dut->pc_wb.read(): %08X\n", dut->pc_wb.read().to_uint());
}

void print_reg_file(const std::unique_ptr<Vbubble_sort_demo>& dut) {
    /*
    const std::array<std::string_view, 31> names {
        "at",
        "v0",
        "v1",
        "a0",
        "a1",
        "a2",
        "a3",
        "t0",
        "t1",
        "t2",
        "t3",
        "t4",
        "t5",
        "t6",
        "t7",
        "s0",
        "s1",
        "s2",
        "s3",
        "s4",
        "s5",
        "s6",
        "s7",
        "t8",
        "t9",
        "k0",
        "k1",
        "gp",
        "sp",
        "s8",
        "ra",
    };
    for(const auto& [name, port]: std::views::zip(names, dut->reg_file)) {
        std::printf("%s: %08X\n", name.data(), port.read().to_uint());
    }
    */
}

void reset(sc_signal<bool>& nrst) {
    for(const auto i: std::ranges::views::iota(0, 3)) {
        sc_start(10, SC_NS);
        sc_start(10, SC_NS);
    }

    sc_start(1, SC_NS);
    nrst = 0;
    sc_start(1, SC_NS);
    nrst = 1;
    sc_start(1, SC_NS);
    nrst = 0;
    sc_start(1, SC_NS);
    nrst = 1;
    sc_start(6, SC_NS);
    sc_start(10, SC_NS);

    nrst = 0;
    sc_start(1, SC_NS);
    nrst = 1;
    sc_start(1, SC_NS);
    nrst = 0;
    sc_start(8, SC_NS);
    sc_start(10, SC_NS);

    sc_start(10, SC_NS);
    sc_start(10, SC_NS);

    sc_start(10, SC_NS);
    sc_start(10, SC_NS);
    nrst = 1;

    sc_start(10, SC_NS);

    sc_start(10, SC_NS);
    sc_start(5, SC_NS);
}

int sc_main(int argc, char* argv[]) {
    Verilated::debug(0);
    Verilated::randReset(2);
    Verilated::traceEverOn(true);
    Verilated::commandArgs(argc, argv);
    std::ios::sync_with_stdio();

    // inputs
    sc_clock clk_100_MHz("clk_100_MHz", sc_time { 10.0, SC_NS });
    sc_signal<bool> nrst;
    sc_signal<bool> stall;
    sc_signal<bool> turbo;
    sc_signal<bool> step;
    sc_signal<bool> show_pc_wb;
    sc_signal<bool> show_stack_array;
    sc_signal<bool> show_ra;
    sc_signal<bool> show_s8;
    sc_signal<bool> show_sp;

    // outputs
    sc_signal<sc_bv<8>> an;
    sc_signal<sc_bv<7>> seg;
    sc_signal<bool> dp;

    const std::unique_ptr<Vbubble_sort_demo> dut{new Vbubble_sort_demo{"bubble_sort_demo_context"}};

    // inputs
    dut->clk_100_MHz(clk_100_MHz);
    dut->nrst(nrst);
    dut->stall(stall);
    dut->turbo(turbo);
    dut->step(step);
    dut->show_pc_wb(show_pc_wb);
    dut->show_stack_array(show_stack_array);
    dut->show_ra(show_ra);
    dut->show_s8(show_s8);
    dut->show_sp(show_sp);

    // outputs
    dut->an(an);
    dut->seg(seg);
    dut->dp(dp);

    nrst = 1;
    stall = 0;
    turbo = 1;
    step = 0;
    show_pc_wb = 0;
    show_stack_array = 0;
    show_ra = 0;
    show_s8 = 0;
    show_sp = 0;

    sc_start(SC_ZERO_TIME);
    Verilated::mkdir("logs");
    tfp = new VerilatedFstSc;
    dut->trace(tfp, 99);
    tfp->open("logs/bubble_sort_demo_tb.fst");
    std::signal(SIGABRT, [](int signal) { if(tfp) { tfp->flush(); tfp->close(); }});
    reset(nrst);

    for(const auto [i, predictor]: std::views::enumerate(Predictor::generate({
        0xA, 0xB, 0xB, 0xA,
        0xB, 0xA, 0xB, 0xA,
    }))) {
        if(i == 0) {
            predictor == dut;
            sc_start(50, SC_US);
        } else {
            sc_start(50, SC_US);
            predictor == dut;
            sc_start(50, SC_US);
        }
    }

    show_pc_wb = 1;
    sc_start(250, SC_US);

    sc_start(1, SC_MS);
    sc_start(1, SC_MS);

    sc_start(1, SC_MS);
    sc_start(1, SC_MS);

    sc_start(1, SC_MS);
    sc_start(1, SC_MS);

    // pc_wb == 0
    sc_start(1, SC_MS);
    sc_start(1, SC_MS);

    sc_start(28, SC_MS);
    sc_start(1, SC_MS);

    if(tfp) { tfp->flush(); tfp->close(); }
    dut->final();
    return 0;
}