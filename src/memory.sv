module data_memory (
    input var logic clk,

    input var logic         load                     ,
    input var logic [2-1:0] load_store_data_size_mode,
    input var logic         load_sign_extend         ,
    input var logic         store                    ,

    input var logic [Constants::WIDTH-1:0] address   ,
    input var logic [Constants::WIDTH-1:0] write_data,

    output var logic [Constants::BYTE-1:0] ram [0:Constants::RAM_SIZE-1],
    output var logic [Constants::WIDTH-1:0] read_data
);
    localparam int unsigned ADDRESS_WIDTH = $clog2(Constants::RAM_SIZE);
    logic [ADDRESS_WIDTH-1:0] address_trunc;

    always_ff @ (posedge clk) begin
        if (store) begin
            if (load_store_data_size_mode == Decode::LoadStoreDataSizeMode_WORD) begin
                ram[address_trunc + 0] <= write_data[31:24];
                ram[address_trunc + 1] <= write_data[23:16];
                ram[address_trunc + 2] <= write_data[15:8];
                ram[address_trunc + 3] <= write_data[7:0];
            end else if (load_store_data_size_mode == Decode::LoadStoreDataSizeMode_HALF_WORD) begin
                ram[address_trunc + 2] <= write_data[15:8];
                ram[address_trunc + 3] <= write_data[7:0];
            end else if (load_store_data_size_mode == Decode::LoadStoreDataSizeMode_BYTE) begin
                ram[address_trunc + 3] <= write_data[7:0];
            end
        end
    end

    always_comb begin
        address_trunc = address[ADDRESS_WIDTH-1:0];
        read_data = 0;
        if (load) begin
            if (load_store_data_size_mode == Decode::LoadStoreDataSizeMode_BYTE) begin
                read_data[7:0] = ram[address_trunc + 3];
            end else if (load_store_data_size_mode == Decode::LoadStoreDataSizeMode_HALF_WORD) begin
                read_data[15:0] = {
                    ram[address_trunc + 2],
                    ram[address_trunc + 3]
                };
            end else if (load_store_data_size_mode == Decode::LoadStoreDataSizeMode_WORD) begin
                read_data = {
                    ram[address_trunc + 0],
                    ram[address_trunc + 1],
                    ram[address_trunc + 2],
                    ram[address_trunc + 3]
                };
            end

            if (load_sign_extend) begin
                if (load_store_data_size_mode == Decode::LoadStoreDataSizeMode_BYTE) begin
                    read_data[31:8] = (((ram[address_trunc + 3][7] == 1) ==? (1)) ? (
                        24'hffffff
                    ) : (
                        24'h000000
                    ));
                end else if (load_store_data_size_mode == Decode::LoadStoreDataSizeMode_HALF_WORD) begin
                    read_data[31:16] = (((ram[address_trunc + 2][7] == 1) ==? (1)) ? (
                        16'hffff
                    ) : (
                        16'h0000
                    ));
                end
            end
        end
    end
endmodule

module memory_buffer (
    input var logic clk,
    input var logic rst,

    input var logic [Constants::WIDTH-1:0]          pc_in        ,
    input var logic                                 load_in      ,
    input var logic [Constants::WIDTH-1:0]          read_data_in ,
    input var logic                                 alu_mode_in  ,
    input var logic [Constants::WIDTH-1:0]          alu_result_in,
    input var logic                                 rd_in        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_in,

    output var logic [Constants::WIDTH-1:0]          pc_out        ,
    output var logic                                 load_out      ,
    output var logic [Constants::WIDTH-1:0]          read_data_out ,
    output var logic                                 alu_mode_out  ,
    output var logic [Constants::WIDTH-1:0]          alu_result_out,
    output var logic                                 rd_out        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_out
);
    always_ff @ (posedge clk, negedge rst) begin
        if (!rst) begin
            pc_out         <= 0;
            load_out       <= 0;
            read_data_out  <= 0;
            alu_mode_out   <= 0;
            alu_result_out <= 0;
            rd_out         <= 0;
            rd_address_out <= 0;
        end else begin
            pc_out         <= pc_in;
            load_out       <= load_in;
            read_data_out  <= read_data_in;
            alu_mode_out   <= alu_mode_in;
            alu_result_out <= alu_result_in;
            rd_out         <= rd_in;
            rd_address_out <= rd_address_in;
        end
    end
endmodule

module memory (
    input  var logic                        clk                ,
    input  var logic                        rst                ,
    input  var logic [Constants::BYTE-1:0]  rom     [0:Constants::ROM_SIZE-1],
    input  var logic                        stall              ,

    input var logic                                 rd_wb        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb,
    input var logic [Constants::WIDTH-1:0]          rd_data_wb   ,

    output var logic [Constants::WIDTH-1:0]          pc_mem        ,
    output var logic [Constants::BYTE-1:0] ram [0:Constants::RAM_SIZE-1],
    output var logic                                 load_mem      ,
    output var logic [Constants::WIDTH-1:0]          read_data_mem ,
    output var logic                                 alu_mode_mem  ,
    output var logic [Constants::WIDTH-1:0]          alu_result_mem,
    output var logic                                 rd_mem        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_mem,
    output var logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT - 1-1]
);
    var logic [Constants::WIDTH-1:0] pc_executed           ;

    var logic                                 rd_executed        ;
    var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_executed;

    var logic                        alu_mode_executed  ;
    var logic [Constants::WIDTH-1:0] alu_result_executed;

    var logic [Constants::WIDTH-1:0] rt_data_excuted;

    var logic         load_executed                     ;
    var logic [2-1:0] load_store_data_size_mode_executed;
    var logic         load_sign_extend_executed         ;
    var logic         store_executed                    ;

    execute execute_inst (
        .clk(clk),
        .rst(rst),
        .rom(rom),
        .stall(stall),

        .rd_wb(rd_wb),
        .rd_address_wb(rd_address_wb),
        .rd_data_wb(rd_data_wb),

        .pc_executed(pc_executed),

        .rd_executed(rd_executed),
        .rd_address_executed(rd_address_executed),

        .alu_mode_executed(alu_mode_executed),
        .alu_result_executed(alu_result_executed),

        .rt_data_excuted(rt_data_excuted),

        .load_executed(load_executed),
        .load_store_data_size_mode_executed(load_store_data_size_mode_executed),
        .load_sign_extend_executed(load_sign_extend_executed),
        .store_executed(store_executed),
        .reg_file(reg_file) 
    );

    logic [Constants::WIDTH-1:0] read_data;
    data_memory data_memory_inst (
        .clk (clk),
        .
        load                      (load_executed),
        .load_store_data_size_mode (load_store_data_size_mode_executed),
        .load_sign_extend          (load_sign_extend_executed),
        .store                     (store_executed),
        .
        address    (alu_result_executed),
        .write_data (rt_data_excuted),
        .
        ram(ram),
        .read_data (read_data)
    );

    memory_buffer memory_buffer_inst (
        .clk (clk),
        .rst (rst),
        .
        pc_in          (pc_executed),
        .load_in       (load_executed    ),
        .read_data_in  (read_data),
        .alu_mode_in   (alu_mode_executed),
        .alu_result_in (alu_result_executed),
        .rd_in         (rd_executed),
        .rd_address_in (rd_address_executed),
        .
        pc_out         (pc_mem        ),
        .load_out       (load_mem      ),
        .read_data_out  (read_data_mem ),
        .alu_mode_out   (alu_mode_mem  ),
        .alu_result_out (alu_result_mem),
        .rd_out         (rd_mem        ),
        .rd_address_out (rd_address_mem)
    );
endmodule