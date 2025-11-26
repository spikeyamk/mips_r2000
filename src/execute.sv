package Execute;
    typedef enum logic [2-1:0] {
        ForwarderSource_DECODED = $bits(logic [2-1:0])'(2'b00),
        ForwarderSource_EXECUTED = $bits(logic [2-1:0])'(2'b01),
        ForwarderSource_WB = $bits(logic [2-1:0])'(2'b10)
    } ForwarderSource;
endpackage

module imm_extender (
    input  var logic                              imm               ,
    input  var logic [Constants::IMM_WIDTH-1:0]   imm_value         ,
    input  var logic                              shamt             ,
    input  var logic [Constants::SHAMT_WIDTH-1:0] shamt_value       ,
    input  var logic                              alu_mode          ,
    input  var logic [5-1:0]                      alu_mode_value    ,
    input  var logic                              load              ,
    input  var logic                              store             ,
    input  var logic                              branch            ,
    output var logic [Constants::WIDTH-1:0]       imm_value_extended
);
    always_comb begin
        if (imm) begin
            if (
                (load | store | branch)
                || (
                    alu_mode
                    && (
                        (alu_mode_value == Decode::ALUMode_ADD)
                        || (alu_mode_value == Decode::ALUMode_ADDU)
                        || (alu_mode_value == Decode::ALUMode_SUB)
                        || (alu_mode_value == Decode::ALUMode_SUBU)
                        || (alu_mode_value == Decode::ALUMode_SLT)
                    )
                )
            ) begin
                imm_value_extended = {
                    (((imm_value[15] == 1) ==? (1)) ? (
                        16'hffff
                    ) : (
                        16'h0000
                    )),
                    imm_value
                };
            end else begin
                imm_value_extended = {16'h0000, imm_value};
            end
        end else if (shamt) begin
            imm_value_extended = {27'b0, shamt_value};
        end else begin
            imm_value_extended = 0;
        end
    end
endmodule

module register_forwarder (
    input  var logic [Constants::WIDTH-1:0] r_data_decoded     ,
    input  var logic [Constants::WIDTH-1:0] alu_result_executed,
    input  var logic [Constants::WIDTH-1:0] rd_data_wb         ,
    input  var logic [2-1:0]                selector           ,
    output var logic [Constants::WIDTH-1:0] r_data_forwarded   
);
    always_comb begin
        r_data_forwarded = (((selector) ==? (Execute::ForwarderSource_DECODED)) ? (
            r_data_decoded
        ) : ((selector) ==? (Execute::ForwarderSource_EXECUTED)) ? (
            alu_result_executed
        ) : ((selector) ==? (Execute::ForwarderSource_WB)) ? (
            rd_data_wb
        ) : (
            0
        ));
    end
endmodule

module forwarding_unit (
    input var logic                                 r                  ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] r_address          ,
    input var logic                                 rd_executed        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_executed,
    input var logic                                 rd_wb              ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb      ,

    output var logic [2-1:0] selector
);
    always_comb begin
        selector = Execute::ForwarderSource_DECODED;
        if (r) begin
            if ((rd_executed == 1) && (r_address == rd_address_executed)) begin
                selector = Execute::ForwarderSource_EXECUTED;
            end else if ((rd_wb == 1) && (r_address == rd_address_wb)) begin
                selector = Execute::ForwarderSource_WB;
            end
        end
    end
endmodule

module alu_register_imm_mux (
    input  var logic                        imm               ,
    input  var logic                        shamt             ,
    input  var logic                        branch            ,
    input  var logic [Constants::WIDTH-1:0] rt_data_forwarded ,
    input  var logic [Constants::WIDTH-1:0] imm_value_extended,
    output var logic [Constants::WIDTH-1:0] alu_b         
);
    always_comb begin
        alu_b = (branch ==? 1) ? rt_data_forwarded :
            ((imm ==? 1) || (shamt ==? 1)) ? imm_value_extended :
            rt_data_forwarded;
    end
endmodule

module alu (
    input var logic [Constants::WIDTH-1:0] a             ,
    input var logic [Constants::WIDTH-1:0] b             ,
    input var logic [Constants::WIDTH-1:0] pc            ,
    input var logic                        link          ,
    input var logic                        alu_mode      ,
    input var logic [5-1:0]                alu_mode_value,
    input var logic                        branch        ,
    input var logic [3-1:0]                branch_mode   ,
    input var logic                        lui           ,
    input var logic                        load          ,
    input var logic                        store         ,

    output var logic [Constants::WIDTH-1:0] result,
    output var logic                        branch_result
);
    always_comb begin
        result = 0;
        branch_result = 0;
        if (branch) begin
            branch_result = (
                ((branch_mode) ==? (Decode::BranchMode_BLTZ)) ? (
                    ($signed(a) < $signed(b))
                ) : ((branch_mode) ==? (Decode::BranchMode_BGEZ)) ? (
                    ($signed(a) >= $signed(b))
                ) : ((branch_mode) ==? (Decode::BranchMode_BEQ)) ? (
                    (a == b)
                ) : ((branch_mode) ==? (Decode::BranchMode_BNE)) ? (
                    (a != b)
                ) : ((branch_mode) ==? (Decode::BranchMode_BLEZ)) ? (
                    ($signed(a) <= $signed(b))
                ) : ((branch_mode) ==? (Decode::BranchMode_BGTZ)) ? (
                    ($signed(a) > $signed(b))
                ) : (
                    1'b0
                )
            );
        end
        if (load || store) begin
            result = a + b;
        end else if (alu_mode) begin
            result = (((alu_mode_value) ==? (Decode::ALUMode_ADD)) ? (
                (a + b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_ADDU)) ? (
                ((link ==? 1) ? (pc + 4 + 4) : (a + b))
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SUB)) ? (
                (a - b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SUBU)) ? (
                (a - b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_AND)) ? (
                (a & b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_OR)) ? (
                (a | b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_XOR)) ? (
                (a ^ b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_NOR)) ? (
                ~(a | b)
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SLL)) ? (
                ((lui ==? 0) ? (a << b[4:0]) : (b << 16))
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SRL)) ? (
                (a >> b[4:0])
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SRA)) ? (
                (a >>> b[4:0])
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SLT)) ? (
                {31'b0, ($signed(a) < $signed(b))}
            ) : ((alu_mode_value) ==? (Decode::ALUMode_SLTU)) ? (
                {31'b0, (a < b)}
            ) : (
                0
            ));
        end
    end
endmodule

module brancher (
    input var logic [Constants::WIDTH-1:0] pc                      ,
    input var logic                        branch                  ,
    input var logic                        branch_comparison_result,
    input var logic                        link                    ,
    input var logic                        jump                    ,
    input var logic                        target                  ,
    input var logic [26-1:0]               target_value            ,
    input var logic                        rs                      ,
    input var logic [Constants::WIDTH-1:0] rs_data                 ,
    input var logic                        imm                     ,
    input var logic [Constants::WIDTH-1:0] imm_value               ,
    input var logic                        rd                      ,

    output var logic                        branch_taken ,
    output var logic [Constants::WIDTH-1:0] branch_target,
    output var logic                        rd_branched
);
    always_comb begin
        branch_target = 0;
        branch_taken  = (jump | (branch & branch_comparison_result));
        rd_branched = rd;
        if (branch | jump) begin
            if (target) begin
                branch_target = {4'b00, target_value, 2'b00};
            end else if (imm) begin
                branch_target = (pc + 4) + (imm_value <<< 2);
            end else if (rs) begin
                branch_target = rs_data;
            end
            rd_branched = (jump & link) | (branch_taken & link);
        end
    end
endmodule

module execute_buffer (
    input var logic clk,
    input var logic rst,

    input var logic [Constants::WIDTH-1:0] pc_in        ,
    input var logic                        alu_mode_in  ,
    input var logic [Constants::WIDTH-1:0] alu_result_in,

    input var logic [Constants::WIDTH-1:0] rt_data_in,

    input var logic                                 rd_in        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_in,

    input var logic         load_in                     ,
    input var logic [2-1:0] load_store_data_size_mode_in,
    input var logic         load_sign_extend_in         ,
    input var logic         store_in                    ,

    output var logic [Constants::WIDTH-1:0] pc_out        ,
    output var logic                        alu_mode_out  ,
    output var logic [Constants::WIDTH-1:0] alu_result_out,

    output var logic [Constants::WIDTH-1:0] rt_data_out,

    output var logic                                 rd_out        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_out,

    output var logic         load_out                     ,
    output var logic [2-1:0] load_store_data_size_mode_out,
    output var logic         load_sign_extend_out         ,
    output var logic         store_out                
);
    always_ff @ (posedge clk, negedge rst) begin
        if (!rst) begin
            pc_out         <= 0;
            alu_mode_out   <= 0;
            alu_result_out <= 0;

            rt_data_out <= 0;

            rd_out         <= 0;
            rd_address_out <= 0;

            load_out                      <= 0;
            load_store_data_size_mode_out <= 0;
            load_sign_extend_out          <= 0;
            store_out                     <= 0;
        end else begin
            pc_out         <= pc_in;
            alu_mode_out   <= alu_mode_in;
            alu_result_out <= alu_result_in;

            rt_data_out <= rt_data_in;

            rd_out         <= rd_in;
            rd_address_out <= rd_address_in;

            load_out                      <= load_in;
            load_store_data_size_mode_out <= load_store_data_size_mode_in;
            load_sign_extend_out          <= load_sign_extend_in;
            store_out                     <= store_in;
        end
    end
endmodule

module execute (
    input  var logic                        clk                ,
    input  var logic                        rst                ,
    input  var logic [Constants::BYTE-1:0]  rom     [0:Constants::ROM_SIZE-1] ,
    input  var logic                        stall              ,

    input var logic                                 rd_wb        ,
    input var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_wb,
    input var logic [Constants::WIDTH-1:0]          rd_data_wb   ,

    output var logic [Constants::WIDTH-1:0] pc_executed           ,

    output var logic                                 rd_executed        ,
    output var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_executed,

    output var logic                        alu_mode_executed  ,
    output var logic [Constants::WIDTH-1:0] alu_result_executed,

    output var logic [Constants::WIDTH-1:0] rt_data_excuted,

    output var logic         load_executed                     ,
    output var logic [2-1:0] load_store_data_size_mode_executed,
    output var logic         load_sign_extend_executed         ,
    output var logic         store_executed,
    output var logic [Constants::WIDTH-1:0] reg_file [0:Constants::REG_COUNT - 1-1]
);
    var logic [Constants::WIDTH-1:0] pc_decoded;

    var logic                                 rs_decoded;
    var logic [Constants::REG_ADDR_WIDTH-1:0] rs_address_decoded;
    var logic [Constants::WIDTH-1:0]          rs_data_decoded;
    var logic                                 rt_decoded;
    var logic [Constants::REG_ADDR_WIDTH-1:0] rt_address_decoded;
    var logic [Constants::WIDTH-1:0]          rt_data_decoded;
    var logic                                 rd_decoded;
    var logic [Constants::REG_ADDR_WIDTH-1:0] rd_address_decoded;

    var logic                               shamt_decoded;
    var logic [Constants::SHAMT_WIDTH-1:0]  shamt_value_decoded ;
    var logic                               imm_decoded;
    var logic [Constants::IMM_WIDTH-1:0]    imm_value_decoded;
    var logic                               target_decoded;
    var logic [Constants::TARGET_WIDTH-1:0] target_value_decoded;

    var logic         alu_mode_decoded;
    var logic [5-1:0] alu_mode_value_decoded;

    var logic         link_decoded;
    var logic         branch_decoded;
    var logic [3-1:0] branch_mode_decoded;
    var logic         jump_decoded;

    var logic         lui_decoded;
    var logic         load_decoded;
    var logic         load_sign_extend_decoded;
    var logic [2-1:0] load_store_data_size_mode_decoded;
    var logic         store_decoded;

    var logic                        branch_taken_branched;
    var logic [Constants::WIDTH-1:0] branch_target_branched;

    decode decode_inst (
        .clk(clk),
        .rst(rst),
        .rom(rom),
        .stall(stall),
        .branch_taken(branch_taken_branched),
        .branch_target(branch_target_branched),

        .rd_wb(rd_wb),
        .rd_address_wb(rd_address_wb),
        .rd_data_wb(rd_data_wb),

        .pc_decoded(pc_decoded),

        .rs_decoded(rs_decoded),
        .rs_address_decoded(rs_address_decoded),
        .rs_data_decoded(rs_data_decoded),
        .rt_decoded(rt_decoded),
        .rt_address_decoded(rt_address_decoded),
        .rt_data_decoded(rt_data_decoded),
        .rd_decoded(rd_decoded),
        .rd_address_decoded(rd_address_decoded),

        .shamt_decoded(shamt_decoded),
        .shamt_value_decoded(shamt_value_decoded),
        .imm_decoded(imm_decoded),
        .imm_value_decoded(imm_value_decoded),
        .target_decoded(target_decoded),
        .target_value_decoded(target_value_decoded),

        .alu_mode_decoded(alu_mode_decoded),
        .alu_mode_value_decoded(alu_mode_value_decoded),

        .link_decoded(link_decoded),
        .branch_decoded(branch_decoded),
        .branch_mode_decoded(branch_mode_decoded),
        .jump_decoded(jump_decoded),

        .lui_decoded(lui_decoded),
        .load_decoded(load_decoded),
        .load_sign_extend_decoded(load_sign_extend_decoded),
        .load_store_data_size_mode_decoded(load_store_data_size_mode_decoded),
        .store_decoded(store_decoded),
        .reg_file(reg_file)
    );

    logic [Constants::WIDTH-1:0] imm_value_extended;
    imm_extender imm_extender_inst (
        .imm                (imm_decoded       ),
        .imm_value          (imm_value_decoded ),
        .shamt              (shamt_decoded     ),
        .shamt_value        (shamt_value_decoded),
        .alu_mode           (alu_mode_decoded  ),
        .alu_mode_value     (alu_mode_value_decoded),
        .load               (load_decoded),
        .store              (store_decoded),
        .branch             (branch_decoded),
        .imm_value_extended (imm_value_extended)
    );

    logic [2-1:0] forwarder_a_selector;
    forwarding_unit forwarding_unit_a (
        .r                   (rs_decoded          ),
        .r_address           (rs_address_decoded  ),
        .rd_executed         (rd_executed         ),
        .rd_address_executed (rd_address_executed ),
        .rd_wb               (rd_wb               ),
        .rd_address_wb       (rd_address_wb       ),
        .selector            (forwarder_a_selector)
    );
    logic [Constants::WIDTH-1:0] rs_data_forwarded;
    register_forwarder register_forwarder_a (
        .r_data_decoded      (rs_data_decoded     ),
        .alu_result_executed (alu_result_executed ),
        .rd_data_wb          (rd_data_wb          ),
        .selector            (forwarder_a_selector),
        .r_data_forwarded    (rs_data_forwarded   )
    );

    logic [2-1:0] forwarder_b_selector;
    forwarding_unit forwarding_unit_b (
        .r                   (rt_decoded          ),
        .r_address           (rt_address_decoded  ),
        .rd_executed         (rd_executed         ),
        .rd_address_executed (rd_address_executed ),
        .rd_wb               (rd_wb               ),
        .rd_address_wb       (rd_address_wb       ),
        .selector            (forwarder_b_selector)
    );
    logic [Constants::WIDTH-1:0] rt_data_forwarded;
    register_forwarder register_forwarder_b (
        .r_data_decoded      (rt_data_decoded     ),
        .alu_result_executed (alu_result_executed ),
        .rd_data_wb          (rd_data_wb          ),
        .selector            (forwarder_b_selector),
        .r_data_forwarded    (rt_data_forwarded   )
    );
    logic [Constants::WIDTH-1:0] alu_b;
    alu_register_imm_mux alu_register_imm_mux_inst (
        .imm                (imm_decoded       ),
        .shamt              (shamt_decoded     ),
        .branch             (branch_decoded    ),
        .rt_data_forwarded  (rt_data_forwarded ),
        .imm_value_extended (imm_value_extended),
        .alu_b              (alu_b             )
    );

    logic [Constants::WIDTH-1:0] alu_result;
    logic                        alu_branch_result;
    alu alu_inst (
        .a              (rs_data_forwarded),
        .b              (alu_b            ),
        .pc             (pc_decoded       ),
        .link           (link_decoded     ),
        .alu_mode       (alu_mode_decoded ),
        .alu_mode_value (alu_mode_value_decoded),
        .branch         (branch_decoded   ),
        .branch_mode    (branch_mode_decoded),
        .lui            (lui_decoded      ),
        .load           (load_decoded     ),
        .store          (store_decoded    ),
        .
        result (alu_result),
        .branch_result (alu_branch_result)
    );

    logic rd_branched;
    brancher brancher_inst (
        .pc                       (pc_decoded        ),
        .branch                   (branch_decoded    ),
        .branch_comparison_result (alu_branch_result ),
        .link                     (link_decoded      ),
        .jump                     (jump_decoded      ),
        .target                   (target_decoded    ),
        .target_value             (target_value_decoded),
        .rs                       (rs_decoded        ),
        .rs_data                  (rs_data_forwarded ),
        .imm                      (imm_decoded       ),
        .imm_value                (imm_value_extended),
        .rd                       (rd_decoded        ),
        .
        branch_taken   ( branch_taken_branched),
        .branch_target (branch_target_branched),
        .rd_branched   (rd_branched           )
    );

    execute_buffer execute_buffer_inst (
        .clk (clk),
        .rst (rst),
        .
        pc_in         (pc_decoded ),
        .alu_mode_in   (alu_mode_decoded),
        .alu_result_in (alu_result),
        .
        rt_data_in (rt_data_forwarded),
        .
        rd_in          (rd_branched),
        .rd_address_in (rd_address_decoded),
        .
        load_in                      (load_decoded),
        .load_store_data_size_mode_in (load_store_data_size_mode_decoded),
        .load_sign_extend_in          (load_sign_extend_decoded),
        .store_in                     (store_decoded),
        .
        pc_out         (pc_executed        ),
        .alu_mode_out   (alu_mode_executed  ),
        .alu_result_out (alu_result_executed),
        .
        rt_data_out (rt_data_excuted),
        .
        rd_out         (rd_executed        ),
        .rd_address_out (rd_address_executed),
        .
        load_out                       (load_executed                     ),
        .load_store_data_size_mode_out (load_store_data_size_mode_executed),
        .load_sign_extend_out          (load_sign_extend_executed         ),
        .store_out                     (store_executed                    )
    );
endmodule