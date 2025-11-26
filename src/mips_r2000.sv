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
    input  var logic                        rst                ,
    input  var logic [Constants::BYTE-1:0]  rom     [0:Constants::ROM_SIZE-1],
    input  var logic                        stall              ,

    output var logic [Constants::WIDTH-1:0]          pc_wb        ,
    output var logic [Constants::BYTE-1:0] ram [0:Constants::RAM_SIZE-1],
    output var logic                                 rd_wb        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb,
    output var logic [Constants::WIDTH-1:0]          rd_data_wb,
    output var logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT - 1-1]
);
    var logic                                 load_mem      ;
    var logic [Constants::WIDTH-1:0]          read_data_mem ;
    var logic                                 alu_mode_mem  ;
    var logic [Constants::WIDTH-1:0]          alu_result_mem;

    memory memory_inst (
        .clk(clk),
        .rst(rst),
        .rom(rom),
        .stall(stall),

        .rd_wb(rd_wb),
        .rd_address_wb(rd_address_wb),
        .rd_data_wb(rd_data_wb),

        .pc_mem(pc_wb),
        .ram(ram),
        .load_mem(load_mem),
        .read_data_mem(read_data_mem),
        .alu_mode_mem(alu_mode_mem),
        .alu_result_mem(alu_result_mem),
        .rd_mem(rd_wb),
        .rd_address_mem(rd_address_wb),
        .reg_file(reg_file)
    );

    writeback writeback_inst (
        .load(load_mem),
        .alu_mode(alu_mode_mem),
        .read_data(read_data_mem),
        .alu_result(alu_result_mem),

        .rd_data_wb(rd_data_wb)
    );
endmodule