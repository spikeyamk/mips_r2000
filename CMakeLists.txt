cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
project(mips_r2000 CXX)

find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT} REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(SystemCLanguage REQUIRED)

function(add_systemc_tb TB_NAME TB_SOURCE)
    set(EXE_NAME ${CMAKE_PROJECT_NAME}_${TB_NAME}_tb)
    add_executable(${EXE_NAME} ${TB_SOURCE})
    target_compile_features(${EXE_NAME} PUBLIC cxx_std_23)
    set(SV_SOURCES ${ARGN})
    verilate(${EXE_NAME}
        SYSTEMC
        TRACE_FST
        VERILATOR_ARGS -pins-bv 2
        SOURCES ${SV_SOURCES}
    )
    verilator_link_systemc(${EXE_NAME})
endfunction()

add_systemc_tb(fetch tb/fetch.cpp src/fetch.sv src/constants.sv)
add_systemc_tb(decode tb/decode.cpp src/decode.sv src/constants.sv src/fetch.sv)
add_systemc_tb(execute tb/execute.cpp src/execute.sv src/constants.sv src/decode.sv src/fetch.sv)
add_systemc_tb(memory tb/memory.cpp src/memory.sv src/constants.sv src/execute.sv src/decode.sv src/fetch.sv)
add_systemc_tb(writeback tb/writeback.cpp src/mips_r2000.sv src/constants.sv src/memory.sv src/execute.sv src/decode.sv src/fetch.sv)
add_systemc_tb(bubble_sort tb/bubble_sort.cpp src/mips_r2000.sv src/constants.sv src/memory.sv src/execute.sv src/decode.sv src/fetch.sv)