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
    input var logic nrst,

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
    always_ff @ (posedge clk, negedge nrst) begin
        if (!nrst) begin
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
    input  var logic                        nrst                ,
    input  var logic [Constants::BYTE-1:0]  rom     [0:Constants::ROM_SIZE-1],
    input  var logic                        stall              ,

    input var logic                                 rd_wb        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb,
    input var logic [Constants::WIDTH-1:0]          rd_data_wb   ,

    output var logic [Constants::WIDTH-1:0]          pc_me        ,
    output var logic [Constants::BYTE-1:0] ram [0:Constants::RAM_SIZE-1],
    output var logic                                 load_me      ,
    output var logic [Constants::WIDTH-1:0]          read_data_me ,
    output var logic                                 alu_mode_me  ,
    output var logic [Constants::WIDTH-1:0]          alu_result_me,
    output var logic                                 rd_me        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_me,
    output var logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT - 1-1]
);
    var logic [Constants::WIDTH-1:0] pc_ex           ;

    var logic                                 rd_ex        ;
    var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_ex;

    var logic                        alu_mode_ex  ;
    var logic [Constants::WIDTH-1:0] alu_result_ex;

    var logic [Constants::WIDTH-1:0] rt_data_ex;

    var logic         load_ex                     ;
    var logic [2-1:0] load_store_data_size_mode_ex;
    var logic         load_sign_extend_ex         ;
    var logic         store_ex                    ;

    execute execute_inst (
        .clk(clk),
        .nrst(nrst),
        .rom(rom),
        .stall(stall),

        .rd_wb(rd_wb),
        .rd_address_wb(rd_address_wb),
        .rd_data_wb(rd_data_wb),

        .pc_ex(pc_ex),

        .rd_ex(rd_ex),
        .rd_address_ex(rd_address_ex),

        .alu_mode_ex(alu_mode_ex),
        .alu_result_ex(alu_result_ex),

        .rt_data_ex(rt_data_ex),

        .load_ex(load_ex),
        .load_store_data_size_mode_ex(load_store_data_size_mode_ex),
        .load_sign_extend_ex(load_sign_extend_ex),
        .store_ex(store_ex),
        .reg_file(reg_file) 
    );

    logic [Constants::WIDTH-1:0] read_data;
    data_memory data_meory_inst (
        .clk (clk),
        .
        load                      (load_ex),
        .load_store_data_size_mode (load_store_data_size_mode_ex),
        .load_sign_extend          (load_sign_extend_ex),
        .store                     (store_ex),
        .
        address    (alu_result_ex),
        .write_data (rt_data_ex),
        .
        ram(ram),
        .read_data (read_data)
    );

    memory_buffer memory_buffer_inst (
        .clk (clk),
        .nrst (nrst),
        .
        pc_in          (pc_ex),
        .load_in       (load_ex    ),
        .read_data_in  (read_data),
        .alu_mode_in   (alu_mode_ex),
        .alu_result_in (alu_result_ex),
        .rd_in         (rd_ex),
        .rd_address_in (rd_address_ex),
        .
        pc_out         (pc_me        ),
        .load_out       (load_me      ),
        .read_data_out  (read_data_me ),
        .alu_mode_out   (alu_mode_me  ),
        .alu_result_out (alu_result_me),
        .rd_out         (rd_me        ),
        .rd_address_out (rd_address_me)
    );
endmodule