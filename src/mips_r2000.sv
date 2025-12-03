module writeback (
    input var logic                                 load      ,
    input var logic                                 alu_mode  ,
    input var logic [Constants::WIDTH-1:0]          read_data ,
    input var logic [Constants::WIDTH-1:0]          alu_result,

    output var logic [Constants::WIDTH-1:0]         rd_data_wb   
);
    always_comb begin
        rd_data_wb     = 0;
        if (load) begin
            rd_data_wb = read_data;
        end else if (alu_mode) begin
            rd_data_wb = alu_result;
        end
    end
endmodule

module mips_r2000 (
    input  var logic                        clk                ,
    input  var logic                        nrst               ,
    input  var logic [Constants::BYTE-1:0]  rom     [0:Constants::ROM_SIZE-1],
    input  var logic                        stall              ,

    output var logic [Constants::WIDTH-1:0]          pc_wb        ,
    output var logic [Constants::BYTE-1:0] ram [0:Constants::RAM_SIZE-1],
    output var logic                                 rd_wb        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb,
    output var logic [Constants::WIDTH-1:0]          rd_data_wb,
    output var logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT - 1-1]
);
    var logic                                 load_me      ;
    var logic [Constants::WIDTH-1:0]          read_data_me ;
    var logic                                 alu_mode_me  ;
    var logic [Constants::WIDTH-1:0]          alu_result_me;

    memory memory_inst (
        .clk(clk),
        .nrst(nrst),
        .rom(rom),
        .stall(stall),

        .rd_wb(rd_wb),
        .rd_address_wb(rd_address_wb),
        .rd_data_wb(rd_data_wb),

        .pc_me(pc_wb),
        .ram(ram),
        .load_me(load_me),
        .read_data_me(read_data_me),
        .alu_mode_me(alu_mode_me),
        .alu_result_me(alu_result_me),
        .rd_me(rd_wb),
        .rd_address_me(rd_address_wb),
        .reg_file(reg_file)
    );

    writeback writeback_inst (
        .load(load_me),
        .alu_mode(alu_mode_me),
        .read_data(read_data_me),
        .alu_result(alu_result_me),

        .rd_data_wb(rd_data_wb)
    );
endmodule