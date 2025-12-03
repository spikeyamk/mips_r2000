#include <memory>
#include <systemc>
#include <ranges>
#include <csignal>
#include <vector>
#include <verilated.h>
#include <verilated_fst_sc.h>
#include "Vfetch.h"
#include "util.hpp"

using namespace sc_core;
using namespace sc_dt;

VerilatedFstSc* tfp = nullptr;

struct Fetch {
    static constexpr uint32_t PC_RESET_VALUE = 0xffff'fffc;
};

int sc_main(int argc, char* argv[]) {
    Verilated::debug(0);
    Verilated::randReset(2);
    Verilated::traceEverOn(true);
    Verilated::commandArgs(argc, argv);
    std::ios::sync_with_stdio();

    sc_clock clk{ "clk", sc_time { 10.0, SC_NS }, 0.5, sc_time { 3.0, SC_NS } };
    sc_signal<bool> nrst;
    sc_signal<bool> stall;
    sc_signal<bool> branch_taken_ex;
    sc_signal<sc_bv<32>> branch_target_ex;
    const uint8_t ROM[] = {
        0x27,0xbd,0xff,0xf0,
        0xaf,0xbe,0x00,0x0c,
        0x03,0xa0,0xf0,0x25,
        0x10,0x00,0x00,0x06,
        0xaf,0xc0,0x00,0x00,
        0xff,0xff,0xff,0xff,
        0x8f,0xc2,0x00,0x00,
        0xff,0xff,0xff,0xff,
        0x24,0x42,0x00,0x01,
        0xaf,0xc2,0x00,0x00,
        0x8f,0xc2,0x00,0x00,
        0xff,0xff,0xff,0xff,
        0x28,0x42,0x00,0x0a,
        0x14,0x40,0xff,0xf7,
        0xff,0xff,0xff,0xff,
        0x10,0x00,0x00,0x0a,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0x8f,0xc2,0x00,0x00,
        0xff,0xff,0xff,0xff,
        0x24,0x42,0xff,0xff,
        0xaf,0xc2,0x00,0x00,
        0x8f,0xc2,0x00,0x00,
        0xff,0xff,0xff,0xff,
        0x00,0x02,0x17,0xc0,
        0xaf,0xc2,0x00,0x00,
        0x8f,0xc2,0x00,0x00,
        0xff,0xff,0xff,0xff,
        0x04,0x40,0xff,0xf4,
        0xff,0xff,0xff,0xff,
        0x0b,0xea,0xee,0x87,
        0xff,0xff,0xff,0xff,
        0x00,0x00,0x10,0x25,
        0x03,0xc0,0xe8,0x25,
        0x8f,0xbe,0x00,0x0c,
        0x03,0xe0,0x00,0x08,
        0x27,0xbd,0x00,0x10
    };
    static_assert((sizeof(ROM) > 4) && ((sizeof(ROM) % 4) == 0));
    std::vector<sc_signal<sc_bv<8>>> rom(std::extent_v<std::remove_reference_t<decltype(Vfetch::rom)>>);
    sc_signal<sc_bv<32>> pc_if;
    sc_signal<sc_bv<32>> instruction_if;

    const std::unique_ptr<Vfetch> dut{new Vfetch{"fetch_context"}};

    dut->clk(clk);
    dut->nrst(nrst);
    dut->stall(stall);
    dut->branch_taken_ex(branch_taken_ex);
    dut->branch_target_ex(branch_target_ex);
    for(const auto& [port, sig]: std::views::zip(dut->rom, rom)) {
        port(sig);
    }
    dut->pc_if(pc_if);
    dut->instruction_if(instruction_if);

    nrst = 1;
    stall = 0;
    branch_taken_ex = 0;
    branch_target_ex = 0;
    for(const auto& [data, sig]: std::views::zip(ROM, rom)) {
        sig = data;
    }
    sc_start(SC_ZERO_TIME);
    Verilated::mkdir("logs");
    tfp = new VerilatedFstSc;
    dut->trace(tfp, 99);
    tfp->open("logs/fetch_tb.fst");
    std::signal(SIGABRT, [](int signal) { if(tfp) { tfp->flush(); tfp->close(); }});

    sc_start(1, SC_NS);
    nrst = 0;
    stall = 1;
    sc_start(1, SC_NS);
    assert(dut->pc_if.read() == Fetch::PC_RESET_VALUE);
    assert(dut->instruction_if.read() == 0);
    nrst = 1;
    sc_start(1, SC_NS);



    stall = 1;
    sc_start(5, SC_NS);
    assert(dut->pc_if.read() == 0);
    assert(dut->instruction_if.read() == 0);
    sc_start(5, SC_NS);

    stall = 1;
    sc_start(5, SC_NS);
    assert(dut->pc_if.read() == 0);
    assert(dut->instruction_if.read() == 0);
    sc_start(5, SC_NS);

    stall = 1;
    sc_start(5, SC_NS);
    assert(dut->pc_if.read() == 0);
    assert(dut->instruction_if.read() == 0);
    sc_start(5, SC_NS);



    stall = 0;
    for(const auto& [i, chunk]: std::views::enumerate(dut->rom | std::views::chunk(4))) {
        sc_start(5, SC_NS);
        assert(dut->pc_if.read() == i * 4);
        assert(dut->instruction_if.read() == cc(chunk[0].read(), chunk[1].read(), chunk[2].read(), chunk[3].read()));
        sc_start(5, SC_NS);
    }

    sc_start(1, SC_NS);
    nrst = 0;
    sc_start(8, SC_NS);
    assert(dut->pc_if.read() == Fetch::PC_RESET_VALUE);
    assert(dut->instruction_if.read() == 0);
    nrst = 1;
    sc_start(1, SC_NS);

    constexpr size_t STALLER { 40 };
    static_assert((STALLER < sizeof(ROM)) && ((STALLER % 4) == 0));

    for(const auto& [i, chunk]: std::views::enumerate(dut->rom | std::views::take(STALLER) | std::views::chunk(4))) {
        sc_start(5, SC_NS);
        assert(dut->pc_if.read() == i * 4);
        assert(dut->instruction_if.read() == cc(chunk[0].read(), chunk[1].read(), chunk[2].read(), chunk[3].read()));
        sc_start(5, SC_NS);
    }

    constexpr size_t BRANCH_TARGET { STALLER * 2 };
    static_assert((BRANCH_TARGET < sizeof(ROM)) && ((BRANCH_TARGET % 4) == 0));
    branch_target_ex = BRANCH_TARGET;

    stall = 1;
    branch_taken_ex = 0;
    sc_start(5, SC_NS);
    assert(dut->pc_if.read() == STALLER);
    assert(dut->instruction_if.read() == 0);
    sc_start(5, SC_NS);

    stall = 1;
    branch_taken_ex = 0;
    sc_start(5, SC_NS);
    assert(dut->pc_if.read() == STALLER);
    assert(dut->instruction_if.read() == 0);
    sc_start(5, SC_NS);

    stall = 1;
    branch_taken_ex = 1;
    sc_start(5, SC_NS);
    assert(dut->pc_if.read() == STALLER);
    assert(dut->instruction_if.read() == 0);
    sc_start(5, SC_NS);

    stall = 0;
    branch_taken_ex = 0;
    for(const auto& [i, chunk]: std::views::enumerate(dut->rom | std::views::drop(BRANCH_TARGET + 4) | std::views::chunk(4))) {
        sc_start(5, SC_NS);
        assert(dut->pc_if.read() == BRANCH_TARGET + 4 + (i * 4));
        assert(dut->instruction_if.read() == cc(chunk[0].read(), chunk[1].read(), chunk[2].read(), chunk[3].read()));
        sc_start(5, SC_NS);
    }

    if(tfp) { tfp->flush(); tfp->close(); }
    dut->final();
    return 0;
}