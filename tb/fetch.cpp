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

int sc_main(int argc, char* argv[]) {
    Verilated::debug(0);
    Verilated::randReset(2);
    Verilated::traceEverOn(true);
    Verilated::commandArgs(argc, argv);
    std::ios::sync_with_stdio();

    sc_clock clk{ "clk", sc_time { 10.0, SC_NS }, 0.5, sc_time { 3.0, SC_NS } };
    sc_signal<bool> rst;
    sc_signal<bool> stall;
    sc_signal<bool> branch_taken;
    sc_signal<sc_bv<32>> branch_target;
    const uint8_t ROM[] = {
        0x27,0xbd,0xff,0xf0,
        0xaf,0xbe,0x00,0x0c,
        0x03,0xa0,0xf0,0x25,
        0x10,0x00,0x00,0x06,
        0xaf,0xc0,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x8f,0xc2,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x24,0x42,0x00,0x01,
        0xaf,0xc2,0x00,0x00,
        0x8f,0xc2,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x28,0x42,0x00,0x0a,
        0x14,0x40,0xff,0xf7,
        0x00,0x00,0x00,0x00,
        0x10,0x00,0x00,0x0a,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x8f,0xc2,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x24,0x42,0xff,0xff,
        0xaf,0xc2,0x00,0x00,
        0x8f,0xc2,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x02,0x17,0xc0,
        0xaf,0xc2,0x00,0x00,
        0x8f,0xc2,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x04,0x40,0xff,0xf4,
        0x00,0x00,0x00,0x00,
        0x0b,0xea,0xee,0x87,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x10,0x25,
        0x03,0xc0,0xe8,0x25,
        0x8f,0xbe,0x00,0x0c,
        0x03,0xe0,0x00,0x08,
        0x27,0xbd,0x00,0x10
    };
    static_assert((sizeof(ROM) > 4) && ((sizeof(ROM) % 4) == 0));
    std::vector<sc_signal<sc_bv<8>>> rom(std::extent_v<std::remove_reference_t<decltype(Vfetch::rom)>>);
    sc_signal<sc_bv<32>> pc_fetched;
    sc_signal<sc_bv<32>> instruction_fetched;

    const std::unique_ptr<Vfetch> dut{new Vfetch{"fetch_context"}};

    dut->clk(clk);
    dut->rst(rst);
    dut->stall(stall);
    dut->branch_taken(branch_taken);
    dut->branch_target(branch_target);
    for(const auto& [port, sig]: std::views::zip(dut->rom, rom)) {
        port(sig);
    }
    dut->pc_fetched(pc_fetched);
    dut->instruction_fetched(instruction_fetched);

    rst = 1;
    stall = false;
    branch_taken = 0;
    branch_target = 0;
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
    rst = 0;
    sc_start(1, SC_NS);
    assert(dut->pc_fetched.read() == 0xffff'fffb);
    assert(dut->instruction_fetched.read() == 0);
    rst = 1;
    sc_start(1, SC_NS);

    for(const auto& [i, chunk]: std::views::enumerate(dut->rom | std::views::chunk(4))) {
        sc_start(5, SC_NS);
        assert(dut->pc_fetched.read() == i * 4);
        assert(dut->instruction_fetched.read() == cc(chunk[0].read(), chunk[1].read(), chunk[2].read(), chunk[3].read()));
        sc_start(5, SC_NS);
    }

    sc_start(1, SC_NS);
    rst = 0;
    sc_start(8, SC_NS);
    assert(dut->pc_fetched.read() == 0xffff'fffb);
    assert(dut->instruction_fetched.read() == 0);
    rst = 1;
    sc_start(1, SC_NS);

    constexpr size_t STALLER { 40 };
    static_assert((STALLER < sizeof(ROM)) && ((STALLER % 4) == 0));

    for(const auto& [i, chunk]: std::views::enumerate(dut->rom | std::views::take(STALLER) | std::views::chunk(4))) {
        sc_start(5, SC_NS);
        assert(dut->pc_fetched.read() == i * 4);
        assert(dut->instruction_fetched.read() == cc(chunk[0].read(), chunk[1].read(), chunk[2].read(), chunk[3].read()));
        sc_start(5, SC_NS);
    }

    constexpr size_t BRANCH_TARGET { STALLER * 2};
    static_assert((BRANCH_TARGET < sizeof(ROM)) && ((BRANCH_TARGET % 4) == 0));
    branch_target = BRANCH_TARGET;

    stall = 1;
    branch_taken = 0;
    sc_start(5, SC_NS);
    assert(dut->pc_fetched.read() == STALLER);
    assert(dut->instruction_fetched.read() == 0);
    sc_start(5, SC_NS);

    stall = 1;
    branch_taken = 0;
    sc_start(5, SC_NS);
    assert(dut->pc_fetched.read() == STALLER);
    assert(dut->instruction_fetched.read() == 0);
    sc_start(5, SC_NS);

    stall = 1;
    branch_taken = 1;
    sc_start(5, SC_NS);
    assert(dut->pc_fetched.read() == STALLER);
    assert(dut->instruction_fetched.read() == 0);
    sc_start(5, SC_NS);

    stall = 0;
    branch_taken = 0;
    for(const auto& [i, chunk]: std::views::enumerate(dut->rom | std::views::drop(BRANCH_TARGET + 4) | std::views::chunk(4))) {
        sc_start(5, SC_NS);
        assert(dut->pc_fetched.read() == BRANCH_TARGET + 4 + (i * 4));
        assert(dut->instruction_fetched.read() == cc(chunk[0].read(), chunk[1].read(), chunk[2].read(), chunk[3].read()));
        sc_start(5, SC_NS);
    }

    if(tfp) { tfp->flush(); tfp->close(); }
    dut->final();
    return 0;
}